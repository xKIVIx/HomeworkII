#include "Server.h"
#include <iostream>


CServer::CServer() {
    int result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData_ );

    if ( result != 0 ) {
        std::cout << "Error init wsa\n";
        return;
    }
    isWsaInit = true;

    struct addrinfo hints;
    ZeroMemory( &hints, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    result = getaddrinfo( "127.0.0.1", "8000", &hints, &listenAddr_ );

    if ( result != 0 ) {
        std::cout << "getaddrinfo failed: " << result << "\n";
        return;
    }

    listenSocet_ = socket( listenAddr_->ai_family, listenAddr_->ai_socktype,
                           listenAddr_->ai_protocol );
    if ( listenSocet_ == INVALID_SOCKET ) {
        std::cout << "Error at socket: " << WSAGetLastError() << "\n";
        return;
    }

    result = bind( listenSocet_, listenAddr_->ai_addr, ( int ) listenAddr_->ai_addrlen );
    if ( result == SOCKET_ERROR ) {
        std::cout << "bind failed with error: " << WSAGetLastError() << "\n";
        return;
    }
}


CServer::~CServer() {
    freeaddrinfo( listenAddr_ );
    closesocket( listenSocet_ );
    WSACleanup();
}

std::string CServer::getRequest() {
    if ( listen( listenSocet_, SOMAXCONN ) == SOCKET_ERROR ) {
        std::cout << "listen failed with error: " << WSAGetLastError() << "\n";
        return std::string();
    }

    clientSocket_ = accept( listenSocet_, NULL, NULL );
    if ( clientSocket_ == INVALID_SOCKET ) {
        std::cout << "accept failed: " << WSAGetLastError() << "\n";
        return std::string();
    }

    const int max_client_buffer_size = 1024;
    char buf [ max_client_buffer_size ];
    std::string request;

    int result = recv( clientSocket_, buf, max_client_buffer_size, 0 );
    if ( result == SOCKET_ERROR ) {
        std::cout << "recv failed: " << result << "\n";
        closesocket( clientSocket_ );
    } else if ( result == 0 ) {
        throw SERVER_ERROR_DISCONECT;
    } else if ( result > 0 ) {
        unsigned int id = 0;
        while ( buf [ id ] != '/' )
            id++;
        id++;
        for ( ; buf [ id ] != ' '; id++ ) {
            request += buf [ id ];
        }
    }
    return request;
}

void CServer::sendResponse( std::string response ) {
    std::string responseHead = "HTTP/1.1 200 OK\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: ";
    responseHead += response.size();
    responseHead += "\r\n\r\n";
    responseHead += response;
    int result = send( clientSocket_, responseHead.c_str(), responseHead.length(), 0 );

    if ( result == SOCKET_ERROR ) {
        std::cout << "send failed: " << WSAGetLastError() << "\n";
    }
    // Закрываем соединение к клиентом
    closesocket( clientSocket_ );
}

#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_ERROR_DISCONECT std::string ("Disconnect..\n");

class CServer {
public:
    CServer();
    ~CServer();
    std::string getRequest();
    void sendResponse( const std::string & response );
private:
    WSADATA wsaData_;
    struct addrinfo* listenAddr_ = nullptr;
    int listenSocet_ = INVALID_SOCKET;
    int clientSocket_ = INVALID_SOCKET;
    bool isWsaInit = false;
};


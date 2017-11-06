#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")
class CServer {
public:
    CServer();
    ~CServer();
    std::string getRequest();
    void sendResponse( std::string response );
private:
    WSADATA wsaData_;
    struct addrinfo* listenAddr_ = nullptr;
    int listenSocet_;
    int clientSocket_;
    bool isWsaInit = false;
};


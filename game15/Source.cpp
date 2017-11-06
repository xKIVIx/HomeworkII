#include <iostream>
#include "Game15\CGameHandler.h"
#include "Server.h"
int main() {
    CServer server;
    // Init handler
    Game15::CGameHandler handler;
    while ( true ) {
        std::string request = server.getRequest();
        request = handler.handle( request );
        server.sendResponse( request );
    }
    return 0;
}
#include <iostream>
#include "Game15\CGameHandler.h"
#include "Claster\CClasterHandler.h"
#include "Server.h"
#include <iostream>
int main() {
    CServer server;
    // Init handler
    Game15::CGameHandler gameHandler;
    Claster::CClasterHandler clasterHandler;
    gameHandler.addNewHandler( (CHandler *)&clasterHandler );
    while ( true ) {
        try {
            std::string request = server.getRequest();
            request = gameHandler.handle( request );
            server.sendResponse( request );
        } catch ( std::string err ) {
            server = CServer();
            std::cout << err;
        };

    }
    return 0;
}
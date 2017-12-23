#include <iostream>
#include "Game15\CGameHandler.h"
#include "Claster\CClasterHandler.h"
#include "VanHao\CVanHaoHandler.h"
#include "Tests\CTestsHandler.h"
#include "COpenCl.h"
#include "Server.h"
#include <iostream>
int main() {
    // create server
    CServer server;

    // init opencl
    try {
        COpenCl::init();
    } catch ( int err ) {
        std::cout << "Error init opencl: " << err;
        return err;
    }

    // init handlers
    Game15::CGameHandler gameHandler;
    Claster::CClasterHandler clasterHandler;
    VanHao::CVanHaoHandler vanHaoHandler;
    Tests::CTestsHandler testsHandler;

    // add handlers
    gameHandler.addNewHandler( (CHandler *)&clasterHandler );
    gameHandler.addNewHandler( (CHandler *)&vanHaoHandler );
    gameHandler.addNewHandler( (CHandler *)&testsHandler );

    // work cicle
    while ( true ) {
        try {
            std::string request = server.getRequest();
            request = gameHandler.handle( request );
            server.sendResponse( request );
        } catch ( std::string & err ) {
            server = CServer();
            std::cout << err;
        };

    }
    return 0;
}
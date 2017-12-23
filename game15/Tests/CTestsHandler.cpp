#include "CTestsHandler.h"
namespace Tests {
    std::string CTestsHandler::handle( std::string & message ) {
        if ( message.back() != 't' )
            return nextHandler( message );
        message.pop_back();
        return std::string();

    }
}

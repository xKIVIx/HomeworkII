#include "CClasterHandler.h"
#include "CClasterisatorGraph.h"
#include "CClasterisatorMask.h"
#include <iostream>


namespace Claster {

    std::string CClasterHandler::handle( std::string message ) const {
        if ( message.back() == 'c' ) {
            message.pop_back();
            if ( message.back() == 'g' ) {
                CClasterisatorGraph clasterisator;
                message.pop_back();
                return clasterisator.clasterisation( message );
            } else if ( message.back() == 'm' ) {
                CClasterisatorMask clasterisator;
                message.pop_back();
                return clasterisator.clasterisation( message );
            }
            return std::string("Claster handler: Unknow modificator - " + message.back());
        } else
            return nextHandler( message );
    }
}

#include "CClasterisator.h"
#include <iostream>
#include "ParseState\CParseStateStartRead.h"

namespace Claster {

    void CClasterisator::parseData( std::string data ) {
        std::cout << "Start parse data\n";
        ParseState::CParseState * state = ( ParseState::CParseState * ) new ParseState::CParseStateStartRead();
        do {
            state = state->parse( data, objectsVector_ );
        } while ( state != nullptr );
        for ( auto objectIterator = objectsVector_.begin();
              objectIterator != objectsVector_.end();
              ++objectIterator ) {
            const unsigned int mid = objectIterator->getDataBuffer().size() / 2;
            for ( unsigned i = 0; i < mid; i++ ) {
                std::swap( objectIterator->getDataBuffer() [ i ],
                           objectIterator->getDataBuffer() [ objectIterator->getDataBuffer().size() - 1 - i ] );
            }
        }
        const unsigned int mid = objectsVector_.size() / 2;
        for ( unsigned i = 0; i < mid; i++ ) {
            std::swap( objectsVector_ [ i ],
                       objectsVector_ [ objectsVector_.size() - 1 - i ] );
        }
        std::cout << "Data:\n";
        for ( auto iterator = objectsVector_.begin(); iterator != objectsVector_.end(); ++iterator )
            iterator->viewInfo();
    }

}
 



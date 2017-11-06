#include "CClasterisator.h"
#include <iostream>
#include "ParseState\CParseSate.h"
#include "ParseState\CParseStateStartRead.h"

namespace Claster {

    void CClasterisator::parseData( std::string data ) {
        ParseState::CParseState * state = ( ParseState::CParseState * ) new ParseState::CParseStateStartRead();
        do {
            ParseState::CParseState * tmpState = state->parse( data, objectsVector_ );
            delete state;
            state = tmpState;
        } while ( state != nullptr );
    }

}
 



#include "CParseStateReadeDataName.h"
#include "CParseStateStartRead.h"

namespace Claster {
    namespace ParseState {
        CParseState * CParseStateReadeDataName::parse( std::string & data, std::vector<CObject>& objectsVector ) {
            if ( data.empty() )
                return nullptr;
            if ( data.back() == ']' )
                return ( CParseState * )new CParseStateStartRead();
            objectsVector.back().getName() = data.back() + objectsVector.back().getName();
            data.pop_back();
            return ( CParseState * )new CParseStateReadeDataName();
        }
    }
}

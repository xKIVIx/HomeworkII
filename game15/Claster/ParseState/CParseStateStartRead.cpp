#include "CParseStateStartRead.h"

#include "CParseStateReadPartData.h"

namespace Claster {
    namespace ParseState {
        CParseState * CParseStateStartRead::parse( std::string & data, std::vector<CObject>& objectsVector ) {
            if ( data.empty() )
                return nullptr;
            if ( data.back() == ']' ) {
                objectsVector.emplace_back();
                objectsVector.back().getDataBuffer().emplace_back();
                data.pop_back();
            } else
                throw PARSE_ERROR_UNCORRET_DATA;
            return ( CParseState * )new CParseStateReadPartData();
        }
    }
}

#include "CParseStateReadPartData.h"
#include "CParseStateNewDataPart.h"
#include "CParseStateReadeDataName.h"


namespace Claster {
    namespace ParseState {
        CParseState * CParseStateReadPartData::parse( std::string & data, std::vector<CObject>& objectsVector ) {
            if ( data.empty() )
                throw PARSE_ERROR_UNEXPECTED_END;
            unsigned int i = 1;
            while ( ( data.back() >= '0' ) && ( data.back() <= '9' ) ) {
                objectsVector.back().getDataBuffer().back() += i*atoi( &data.back() );
                data.pop_back();
                if ( data.empty() )
                    throw PARSE_ERROR_UNEXPECTED_END;
                i *= 10;
            }
            if ( i == 1 )
                throw PARSE_ERROR_UNCORRET_DATA;
            if ( data.back() == '+' )
                return ( CParseState * ) new CParseStateNewDataPart();
            if ( data.back() == '[' ) {
                data.pop_back();
                return ( CParseState * ) new CParseStateReadeDataName();
            }

            throw PARSE_ERROR_UNCORRET_DATA;
        }
    }
}

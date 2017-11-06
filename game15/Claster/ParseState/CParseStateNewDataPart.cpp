#include "CParseStateNewDataPart.h"
#include "CParseStateReadPartData.h"
namespace Claster {
    namespace ParseState {
        CParseState * CParseStateNewDataPart::parse( std::string & data, std::vector<CObject>& objectsVector ) {
            objectsVector.back().getDataBuffer().emplace_back();
            objectsVector.pop_back();
            return (CParseState *)new CParseStateReadPartData();
        }
    }
}

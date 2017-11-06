#pragma once
#include "CParseSate.h"
namespace Claster {
    namespace ParseState {
        class CParseStateNewDataPart :CParseState {
        public:
            virtual CParseState * parse( std::string &data, std::vector <CObject> &objectsVector );
        };
    }
}


#pragma once
#include "CParseSate.h"
namespace Claster {
    namespace ParseState {
        class CParseStateReadPartData :CParseState {
        public:
            virtual CParseState * parse( std::string &data, std::vector <CObject> &objectsVector );
        };
    }
}


#pragma once
#include "../CObject.h"
namespace Claster {
    namespace ParseState {
        const char *PARSE_ERROR_UNCORRET_DATA = "Parse error! Uncorrect data.\n",
            *PARSE_ERROR_UNEXPECTED_END = "Parse error! Unexpected end of data.\n";

        class CParseState {
        public:
            virtual CParseState * parse( std::string &data, std::vector <CObject> &objectsVector ) = 0;
        };
    }
}
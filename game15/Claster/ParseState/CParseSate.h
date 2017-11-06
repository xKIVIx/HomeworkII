#include "../CObject.h"
#ifndef CPARSE_STATE_H
#define CPARSE_STATE_H
namespace Claster {
    namespace ParseState {
        #define PARSE_ERROR_UNCORRET_DATA std::string ("Parse error! Uncorrect data.\n")
        #define PARSE_ERROR_UNEXPECTED_END std::string("Parse error! Unexpected end of data.\n")

        class CParseState {
        public:
            virtual ~CParseState();
            virtual CParseState * parse( std::string &data, std::vector <CObject> &objectsVector ) = 0;
        };
    }
}
#endif // !CPARSE_STATE_HEAD


#pragma once
#include "../CHandler.h"
namespace Tests {
    class CTestsHandler : CHandler {
    public:
        virtual std::string handle( const std::string & message );
    };
}

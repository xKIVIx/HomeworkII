#pragma once
#include "../CHandler.h"
namespace Tests {
    class CTestsHandler : CHandler {
    public:
        virtual std::string handle( std::string & message );
    private:
        void * core_ = nullptr;
    };
}

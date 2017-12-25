#pragma once
#include "../CHandler.h"

namespace Tests {
    class CTestsHandler : CHandler {
    public:
        CTestsHandler();
        ~CTestsHandler();
        virtual std::string handle( std::string & message );
    private:
        struct SRowAdress {
            size_t setId_ = 0,
                   rowId_ = 0;
        };
        void * core_ = nullptr;
        std::string newData( std::string & message );
        // TODO
        std::string addData( std::string & message );
        // TODO
        std::string deleteData( std::string & message );

    };
}

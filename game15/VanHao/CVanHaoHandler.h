#pragma once
#include "../CHandler.h"
namespace VanHao {
    class CVanHaoHandler :CHandler {
    public:
        virtual std::string handle( std::string & message );
    };
}


#pragma once
#include "../CHandler.h"

namespace Claster {

    class CClasterHandler : CHandler {
    public:
        virtual std::string handle( std::string message ) const;
    };
}


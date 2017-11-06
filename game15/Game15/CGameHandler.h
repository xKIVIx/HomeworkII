
#ifndef CGAMEHANDLER_H
#define CGAMEHANDLER_H
#include "../CHandler.h"

#include <string>

namespace Game15 {

    class CGameHandler : public CHandler {
    public:


        virtual std::string handle( std::string message ) const;

    };
} // end of package namespace

#endif // CGAMEHANDLER_H

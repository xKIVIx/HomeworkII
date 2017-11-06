#ifndef DownTurn_h
#define DownTurn_h

#include "CTreeElem.h"

namespace Game15 {
    class CDownTurn : public CTreeElem {

    public:

        CDownTurn( CTreeElem * lastElem );

        virtual void countNextTurns();
    };
}
#endif // DownTurn_h

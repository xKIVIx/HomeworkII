#ifndef CLeftTurn_h
#define CLeftTurn_h

#include "CTreeElem.h"

namespace Game15 {
    class CLeftTurn : public CTreeElem {

    public:

        CLeftTurn( CTreeElem * lastElem );

        virtual void countNextTurns();
    };
}
#endif // CLeftTurn_h

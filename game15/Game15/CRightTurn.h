#ifndef CRightTurn_h
#define CRightTurn_h

#include "CTreeElem.h"

namespace Game15 {
    class CRightTurn : public CTreeElem {

    public:

        CRightTurn( CTreeElem * lastElem );

        virtual void countNextTurns();
    };
}
#endif // CRightTurn_h

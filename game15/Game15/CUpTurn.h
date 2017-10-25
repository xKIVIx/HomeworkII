#ifndef CUpTurn_h
#define CUpTurn_h

#include "CTreeElem.h"

namespace Game15
{
    class CUpTurn : public CTreeElem {

    public:

        CUpTurn( CTreeElem * lastElem_ );

        virtual void countNextTurns();
    };
}
#endif // CUpTurn_h

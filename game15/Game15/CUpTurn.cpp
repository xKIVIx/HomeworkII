#include "CUpTurn.h"
#include "CLeftTurn.h"
#include "CRightTurn.h"


namespace Game15
{

    CUpTurn::CUpTurn( CTreeElem * lastElem ) :CTreeElem( lastElem )
    {
        if ( isLock_ )
            return;

        if ( ( positionEmpyCell_ / SIZE_WALL ) == 0 )
        {
            isLock_ = true;
            return;
        }

        positionEmpyCell_ -= SIZE_WALL;
        char tmp;
        tmp = field_ [ ( ( CUpTurn * ) lastElem )->positionEmpyCell_ ];
        field_ [ ( ( CUpTurn * ) lastElem )->positionEmpyCell_ ] = field_ [ positionEmpyCell_ ];
        field_ [ positionEmpyCell_ ] = tmp;

        if ( isCircle() )
        {
            isLock_ = true;
            return;
        }

        countIndentife();
    }

    void CUpTurn::countNextTurns()
    {
        if ( nextTurns_.size() != 0 )
            return;
        CTreeElem * tmp = nullptr;
        nextTurns_.reserve( 3 );
#pragma omp parallel sections private( tmp )
        {

#pragma omp section
            {
                tmp = ( CTreeElem * ) new CLeftTurn( this );
#pragma omp critical (adding)
                {
                    nextTurns_.push_back( tmp );
                }
            }
#pragma omp section
            {
                tmp = ( CTreeElem * ) new CRightTurn( this );
#pragma omp critical (adding)
                {
                    nextTurns_.push_back( tmp );
                }
            }
#pragma omp section
            {
                tmp = ( CTreeElem * ) new CUpTurn( this );
#pragma omp critical (adding)
                {
                    nextTurns_.push_back( tmp );
                }
            }
        }
    }
}

#include "CTreeElem.h"
#include "CUpTurn.h"
#include "CRightTurn.h"
#include "CDownTurn.h"
#include "CLeftTurn.h"
#include <iostream>


namespace Game15 {
    CTreeElem::CTreeElem( unsigned char field [ COUNT_CELL ], unsigned char positionEmpyCell )

    {
        positionEmpyCell_ = positionEmpyCell;
        std::memcpy( field_, field, COUNT_CELL );
    }


    std::string CTreeElem::sreachWay()

    {
        std::cout << "Find answer to : " << parseState();
        std::string result;
        countIndentife();
        CTreeElem * head = this;
        unsigned int countIterations = 0;
        while ( head->indentife_ != 0 ) {
            CTreeElem * tmp = head->sreachNextTurn();
            if ( tmp == nullptr ) {
                head->isLock_ = true;
                head->clearNextTurns();
                head = head->lastTurn_;
                if ( head == nullptr ) {
                    std::cout << "Can`t find answer to : " << parseState();
                    return "Can`t find answer";
                }
                continue;
            }
            head = tmp;
            countIterations++;
            if ( countIterations > MAX_ITERATIONS ) {
                std::cout << "Can`t find answer to : " << parseState();
                return "Can`t find answer";
            }
            std::cout << "Interation : " << countIterations << "  Current lvl : " << unsigned int( head->currentLvl_ ) << std::endl;

        }
        while ( head != nullptr ) {
            result = head->parseState() + result;
            head = head->lastTurn_;
        }
        return result;

    }


    CTreeElem::~CTreeElem() {
        clearNextTurns();
    }

    CTreeElem::CTreeElem( CTreeElem * lastElem ) {
        currentLvl_ = lastElem->currentLvl_ + 1;
        if ( IS_GOD_NUMBER( currentLvl_ ) ) {
            isLock_ = true;
            return;
        }

        memcpy( field_, lastElem->field_, COUNT_CELL );
        lastTurn_ = lastElem;
        positionEmpyCell_ = lastElem->positionEmpyCell_;
    }

    void CTreeElem::clearNextTurns() {
        for ( char i = 0; i < nextTurns_.size(); i++ ) {
            delete nextTurns_ [ i ];
        }
        nextTurns_.reserve( 0 );
        nextTurns_.clear();
    }

    void CTreeElem::countIndentife() {
        indentife_ = 0;
        for ( unsigned char i = 0; i < COUNT_CELL; i++ ) {
            if ( field_ [ i ] != ( i + 1 ) )
                indentife_++;
        }
        indentife_--;
    }

    std::string CTreeElem::parseState() {
        std::string result = "";
        result.reserve( 33 );
        for ( unsigned char i = 0; i < COUNT_CELL; i++ ) {
            result += std::to_string( field_ [ i ] );
            result += ',';
        }
        result.pop_back();
        result += "\n";
        return result;
    }


    void CTreeElem::countNextTurns()

    {
        if ( nextTurns_.size() != 0 )
            return;
        nextTurns_.push_back( ( CTreeElem * ) new CLeftTurn( this ) );
        nextTurns_.push_back( ( CTreeElem * ) new CUpTurn( this ) );
        nextTurns_.push_back( ( CTreeElem * ) new CRightTurn( this ) );
        nextTurns_.push_back( ( CTreeElem * ) new CDownTurn( this ) );

    }


    unsigned char CTreeElem::getDeepIndentife( unsigned char deep )

    {

        if ( isLock_ )
            return UCHAR_MAX;
        if ( IS_GOD_NUMBER( deep + currentLvl_ ) )
            return UCHAR_MAX;
        if ( deep == 0 ) {
            return indentife_;
        } else {
            countNextTurns();

            unsigned char minIndentife = UCHAR_MAX;
            for ( auto iterator = nextTurns_.begin(); iterator != nextTurns_.end(); ++iterator ) {
                unsigned char tmp = ( *iterator )->getDeepIndentife( deep - 1 );
                if ( tmp < minIndentife )
                    minIndentife = tmp;
            }

            return minIndentife;
        }
    }


    bool CTreeElem::isCircle()

    {
        CTreeElem * select = lastTurn_;
        while ( select != nullptr ) {
            unsigned char i = 0;
            for ( ; i < COUNT_CELL; i++ )
                if ( select->field_ [ i ] != field_ [ i ] ) {
                    select = select->lastTurn_;
                    break;
                }

            if ( i == COUNT_CELL )
                return true;
        }
        return false;
    }


    CTreeElem* CTreeElem::sreachNextTurn() {
        std::vector< CTreeElem* > equalsWays;
        equalsWays.reserve( 4 );
        countNextTurns();
        for ( auto iterator = nextTurns_.begin(); iterator != nextTurns_.end(); ++iterator ) {
            if ( ( *iterator )->isLock_ )
                continue;

            if ( equalsWays.size() == 0 ) {
                equalsWays.push_back( *iterator );
                continue;
            }

            if ( equalsWays.back()->indentife_ > ( *iterator )->indentife_ ) {
                equalsWays.clear();
                equalsWays.push_back( *iterator );
                continue;
            }

            if ( equalsWays.back()->indentife_ == ( *iterator )->indentife_ )
                equalsWays.push_back( *iterator );
        }

        if ( equalsWays.size() == 0 )
            return nullptr;

        unsigned char deep = 1;
        std::vector <CTreeElem*> newEqualWays;
        newEqualWays.reserve( equalsWays.size() );
        while ( equalsWays.size() > 1 ) {
            unsigned char * minIndentifes = new unsigned char [ equalsWays.size() ] { UCHAR_MAX };
            newEqualWays.clear();
            #pragma omp parallel for
            for ( int i = 0; i < equalsWays.size(); i++ ) {
                minIndentifes [ i ] = equalsWays [ i ]->getDeepIndentife( deep );
            }
            unsigned char min = UCHAR_MAX;
            for ( unsigned char i = 0; i < equalsWays.size(); i++ ) {
                if ( minIndentifes [ i ] < min ) {
                    newEqualWays.clear();
                    newEqualWays.push_back( equalsWays [ i ] );
                    min = minIndentifes [ i ];
                    continue;
                }

                if ( minIndentifes [ i ] == min ) {
                    newEqualWays.push_back( equalsWays [ i ] );
                    continue;
                }
            }
            delete minIndentifes;
            if ( min == UCHAR_MAX )
                return nullptr;

            equalsWays.swap( newEqualWays );
            deep++;
        }

        return equalsWays.back();
    }
}

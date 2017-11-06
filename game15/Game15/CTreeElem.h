#ifndef CTreeElem_h
#define CTreeElem_h

#include <vector>
#include <string>
namespace Game15 {
    #define IS_GOD_NUMBER(x) x > 31
    #define COUNT_CELL 9
    #define SIZE_WALL 3
    #define MAX_ITERATIONS 181440

    class CTreeElem {
    public:

        CTreeElem( unsigned char field [ COUNT_CELL ], unsigned char positionEmpyCell );

        std::string sreachWay();

        ~CTreeElem();

    protected:
        CTreeElem( CTreeElem * lastElem );

        void clearNextTurns();

        void countIndentife();

        std::string parseState();

        virtual void countNextTurns();

        unsigned char getDeepIndentife( unsigned char deep );

        bool isCircle();

        CTreeElem* sreachNextTurn();


    protected:
        std::vector< CTreeElem* > nextTurns_;
        CTreeElem *lastTurn_ = nullptr;
        unsigned char currentLvl_ = 0;
        unsigned char indentife_;
        unsigned char field_ [ COUNT_CELL ];
        unsigned char positionEmpyCell_;
        bool isLock_ = false;
    };
}
#endif // CTreeElem_h

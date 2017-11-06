#ifndef CCLASTERISATORGRAPH_H
#define CCLASTERISATORGRAPH_H

#include "CClasterisator.h"

namespace Claster {

    class CClasterisatorGraph : public CClasterisator {
    public:

        ~CClasterisatorGraph();

        virtual std::string clasterisation( std::string data );

    private:

        CObject * graphHead_;

        unsigned int ** distinationMatrix_ = nullptr;

        unsigned int countClasters_ = 0,
            min_i = 0,
            min_k = 0;

        void buildLineGraph();

        void clearMatrix();

        void computeMatrix();

    };
}

#endif

#ifndef CCLASTERISATOR_H
#define CCLASTERISATOR_H

#include <string>
#include <vector>

#include "CObject.h"

#define CLASTER_UNCORRECT_DATA std::string("Uncorrect data\n")

namespace Claster {

    class CClasterisator {
    public:



        /**
         * Return string in format : a1+a2;a3;.....
         * @return std::string
         * @param  data the format string : a1[d1+d2+...+dn]...an[....]<options for mask
         * what object is mask, for graph how many clasters><type clasterisation "m" -
         * mask, "g"-graph >
         */
        virtual std::string clasterisation( std::string data ) = 0;


    protected:
        std::vector <CObject> objectsVector_;

        void parseData( std::string data );


    };
} // end of package namespace

#endif // CCLASTERISATOR_H

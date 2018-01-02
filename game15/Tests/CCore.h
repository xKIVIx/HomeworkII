#pragma once
#include <list>
#include <vector>
#include "CBitStorage.h"
namespace Tests {
    class CCore {
    public:
        /*
         * To set data for computing.
         * @data - vector containing lists with bit stores of sets.
         * @sizeStorage - count buffers in bitstorage
         */
        void setData( std::vector <std::list <BitStorage::CBitStorage<byte>>> data );
        std::list<std::list<size_t>> getTests();
    private:
        std::list < BitStorage::CBitStorage<byte> > diffData_;

        std::vector < size_t > setsSize_;

        std::vector < std::list < BitStorage::CBitStorage<byte> > > data_;

        std::list < BitStorage::CBitStorage<byte> > tests_;

        void countDiffData();

        void addDiffData( const byte * data,
                          const size_t countRow,
                          const size_t sizeRow );
        void sreachSkipData( std::list < BitStorage::CBitStorage<byte> > & data );

        void sreachTests();
    };
}
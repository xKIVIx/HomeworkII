#include <ppl.h>
#include "CCore.h"

void Tests::CCore::setData( std::vector<std::list<BitStorage::CBitStorage<byte>>> data ) {
    data_.swap( data );
}

std::list<std::list<size_t>> Tests::CCore::getTests() {
    countDiffData();
    return std::list<std::list<size_t>>();
}

void Tests::CCore::countDiffData() {
    size_t countBuffers = data_.back().back().getCountByffers();
    for ( size_t fSetId = 0; fSetId < data_.size(); fSetId++ ) {
        size_t fDataCountRow = data_ [ fSetId ].size();

        byte * fData = new byte [ fDataCountRow * countBuffers] { 0 };
        size_t writePosition = 0;
        for ( auto elemIter = data_ [ fSetId ].begin();
              elemIter != data_ [ fSetId ].end();
              ++elemIter ) {
            writePosition += elemIter->getBits( &( fData [ writePosition ] ), 
                                                countBuffers );
        }

        for ( size_t sSetId = fSetId + 1; sSetId < data_.size(); sSetId++ ) {

            size_t sDataCountRow = data_ [ fSetId ].size();
            byte * sData = new byte [ sDataCountRow * countBuffers] { 0 };
            // write position for second data
            size_t writePosition = 0;

            for ( auto elemIter = data_ [ sSetId ].begin();
                  elemIter != data_ [ sSetId ].end();
                  ++elemIter ) {            
                writePosition += elemIter->getBits( &( sData [ writePosition ] ), 
                                                    countBuffers );
            }
            byte * result;
            size_t resultCountRow = COpenCl::getInstance().computeDiff( fData,
                                                                        sData,
                                                                        fDataCountRow,
                                                                        sDataCountRow,
                                                                        countBuffers,
                                                                        &result );
            addDiffData( result, resultCountRow, countBuffers );
            delete [] result;
            delete [] sData;
        }
        delete [] fData;
    }
}

void Tests::CCore::addDiffData( const byte * data, 
                                const size_t countRow, 
                                const size_t sizeRow ) {
    for ( size_t i = 0; i < countRow; i++ ) {
        diffData_.push_front( BitStorage::CBitStorage<byte>( sizeRow ) );
        diffData_.front().setBits( data, sizeRow );
    }
}

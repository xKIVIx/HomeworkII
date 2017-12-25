#include <ppl.h>
#include "CCore.h"

void Tests::CCore::setData( std::vector<std::list<BitStorage::CBitStorage<byte>>> data ) {
    data_.swap( data );
    diffData_.clear();
}

std::list<std::list<size_t>> Tests::CCore::getTests() {
    std::cout << "----------Diff matrix----------\n";
    countDiffData();
    std::cout << "-------Sorted diff matrix------\n";
    sreachSkipDiffData();
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

            size_t sDataCountRow = data_ [ sSetId ].size();
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
    diffData_.sort( [] ( const BitStorage::CBitStorage<byte> & one, 
                         const BitStorage::CBitStorage<byte> & two ) {
        return one.getCountOnes() < two.getCountOnes();
    } );
}

void Tests::CCore::addDiffData( const byte * data, 
                                const size_t countRow, 
                                const size_t sizeRow ) {
    for ( size_t i = 0; i < countRow; i++ ) {
        diffData_.push_back( BitStorage::CBitStorage<byte>( sizeRow ) );
        diffData_.back().setBits( &( data [ i*sizeRow ] ), sizeRow );
        diffData_.back().viewStorageData();
    }
}

void Tests::CCore::sreachSkipDiffData() {
    for ( auto iter1 = diffData_.begin(); iter1 != diffData_.end(); ++iter1 ) {
        if ( !iter1->getSkipState() ) {
            iter1->viewStorageData();
            auto iter2 = iter1;
            ++iter2;
            for ( ; iter2 != diffData_.end(); ++iter2 ) {
                if ( !iter2->getSkipState() ) {
                    iter2->setSkipState( iter1->isExtansion( *iter2 ) );
                }
            }
        }
    }
}

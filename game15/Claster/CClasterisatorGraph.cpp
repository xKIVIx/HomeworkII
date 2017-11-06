#include "CClasterisatorGraph.h"
#include <iostream>

Claster::CClasterisatorGraph::~CClasterisatorGraph() {
    clearMatrix();
}

std::string Claster::CClasterisatorGraph::clasterisation( std::string data ) {
    std::cout << "Start graph clasterisation\n";

    countClasters_ = 0;
    for ( unsigned int i = 1; data.back() != ']'; i *= 10 ) {
        countClasters_ += ( i * atoi( &data.back() ) );
        data.pop_back();
        if ( data.size() == 0 ) {
            std::cout << CLASTER_UNCORRECT_DATA << std::endl;
            return CLASTER_UNCORRECT_DATA;
        }
    }


    std::cout << "Start parse data\n";
    try {
        parseData( data );
    } catch ( std::string er ) {
        std::cout << er << std::endl;
        return er;
    }
    std::cout << "Data:\n";
    for ( auto iterator = objectsVector_.begin(); iterator != objectsVector_.end(); ++iterator )
        iterator->viewInfo();
    std::cout << "Count clasters: " << countClasters_ << "\n";

    computeMatrix();

    buildLineGraph();

    for ( unsigned int countSetClasters = 0; countSetClasters < countClasters_; countSetClasters++ ) {
        graphHead_->deleteMaxEdge();
    }
    std::cout << "Clasterisation is succesful\n";
    return graphHead_->getClasters();
}

void Claster::CClasterisatorGraph::buildLineGraph() {
    std::cout << "Build graph\n";
    graphHead_ = &objectsVector_ [ min_i ];
    graphHead_->setNextObject( objectsVector_ [ min_k ] );
    unsigned int countAdded = 2;
    bool * usedVert = new bool [ objectsVector_.size() ] { false };
    usedVert [ min_i ] = true;
    usedVert [ min_k ] = true;

    while ( countAdded < objectsVector_.size() ) {
        min_i = min_k;
        min_k = 0;
        for ( unsigned int k = 0; k < objectsVector_.size(); k++ ) {
            if ( !usedVert [ k ] ) {
                if ( distinationMatrix_ [ min_i ] [ min_k ] > distinationMatrix_ [ min_i ] [ k ] )
                    min_k = k;
            }
        }
        graphHead_->setNextObject( objectsVector_ [ min_k ] );
        countAdded++;
    }
    std::cout << "Graph :" << graphHead_->getClasters() << std::endl;
}

void Claster::CClasterisatorGraph::clearMatrix() {
    if ( distinationMatrix_ != nullptr )
        for ( unsigned int i = 0; i < objectsVector_.size(); i++ ) {
            if ( distinationMatrix_ != nullptr )
                delete [] distinationMatrix_ [ i ];
        }
    delete [] distinationMatrix_;
}

void Claster::CClasterisatorGraph::computeMatrix() {
    clearMatrix();
    std::cout << "Compute distanation matrix\n";
    distinationMatrix_ = new unsigned int * [ objectsVector_.size() ];
    for ( unsigned int i = 0; i < objectsVector_.size(); i++ ) {
        distinationMatrix_ [ i ] = new unsigned int [ objectsVector_.size() ];
        for ( unsigned int k = 0; k < objectsVector_.size(); k++ ) {
            distinationMatrix_ [ i ] [ k ] = objectsVector_ [ k ].getDefferentCount( objectsVector_ [ i ] );
            if ( ( distinationMatrix_ [ i ] [ k ] < distinationMatrix_ [ min_i ] [ min_k ] ) && ( k != i ) ) {
                min_i = i;
                min_k = k;
            }
        }
    }
    std::cout << "Distination matrix:\n";
    for ( unsigned int i = 0; i < objectsVector_.size(); i++ ) {
        for ( unsigned int k = 0; k < objectsVector_.size(); k++ )
            std::cout << distinationMatrix_ [ i ] [ k ] << "\t";
        std::cout << std::endl;
    }

}

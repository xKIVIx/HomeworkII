#include "CClasterisatorMask.h"

#include <iostream>

std::string Claster::CClasterisatorMask::clasterisation( std::string data ) {
    if ( data.empty() )
            return CLASTER_UNCORRECT_DATA;
    unsigned int grad = 1;
    while ( ( data.back() >= '0' ) && ( data.back() <= '9' ) ) {
        minDeff_ += grad * atoi( &data.back() );
        data.pop_back();
        if ( data.empty() )
            return CLASTER_UNCORRECT_DATA;
        grad *= 10;
    }

    try {
        initMasks( data );
        parseData( data );
    } catch ( std::string err ) {
        std::cout << err;
        return err;
    }
    initIdentGroups();
    sreachClasters();
    std::string result;
    for ( auto iter = clastersHeads_.begin(); iter != clastersHeads_.end(); ++iter ) {
        result += ( *iter )->getClasters();
        result += ';';
    }
    std::cout << "Clasteristion is successful.\n";
    return result;
}

void Claster::CClasterisatorMask::initMasks( std::string & data ) {
    unsigned int i = 1;
    if ( data.empty() )
            throw CLASTER_UNCORRECT_DATA;
    while ( data.back() != ']' ) {
        if ( data.empty() )
            throw CLASTER_UNCORRECT_DATA;
        if ( ( data.back() >= '0' ) && ( data.back() <= '9' ) ) {
            masks_.back() += i * atoi( &data.back() );
            i *= 10;
        } else if ( data.back() == '+' ) {
            i = 1;
            masks_.emplace_back( 0 );
        } else {
            throw CLASTER_UNCORRECT_DATA;
        }
        data.pop_back();
    }
}

void Claster::CClasterisatorMask::initIdentGroups() {
    std::cout << "Start init identife marix\nIdentife marix:\nmask\t";
    identGroups_.reserve( objectsVector_.size() );
    for ( auto iter = masks_.begin(); iter != masks_.end(); ++iter ) {
        std::cout << objectsVector_ [ *iter ].getName() << '\t';
    }
    std::cout << std::endl;
    for ( unsigned int i = 0; i < objectsVector_.size(); i++ ) {
        identGroups_.emplace_back();
        identGroups_.back().reserve( masks_.size() );
        std::cout << objectsVector_ [ i ].getName() << '\t';
        for ( auto iter = masks_.begin(); iter != masks_.end(); ++iter ) {
            identGroups_.back().push_back( objectsVector_ [ i ].getDefferentCount( objectsVector_ [ *iter ] ) <
                                           minDeff_ );
            std::cout << unsigned int( identGroups_.back().back() ) << '\t';
        }
        std::cout << std::endl;
    }

}

void Claster::CClasterisatorMask::sreachClasters() {
    bool * usedGroup = new bool [ identGroups_.size() ] { false };
    for ( unsigned int i = 0; i < identGroups_.size(); i++ ) {
        if ( usedGroup [ i ] )
            continue;
        clastersHeads_.push_back( &objectsVector_ [ i ] );
        for ( unsigned int k = i + 1; k < identGroups_.size(); k++ ) {
            if ( isEqualGroup( i, k ) ) {
                usedGroup [ k ] = true;
                clastersHeads_.back()->setNextObject( objectsVector_ [ k ] );
            }
        }
    }
    delete [] usedGroup;
}

bool Claster::CClasterisatorMask::isEqualGroup( unsigned int firstGroup, unsigned int secondGroup ) {
    for ( unsigned int i = 0; i < identGroups_ [ firstGroup ].size(); i++ ) {
        if ( identGroups_ [ firstGroup ] [ i ] != identGroups_ [ secondGroup ] [ i ] )
            return false;
    }
    return true;
}

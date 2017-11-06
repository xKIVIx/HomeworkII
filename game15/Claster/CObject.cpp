#include "CObject.h"
#include <iostream>

// Constructors/Destructors
//  


/**
* Set data
* @param data the new data
* @param size the size new data
*/

Claster::CObject::CObject() {
}

Claster::CObject::~CObject() {
}

unsigned int Claster::CObject::getDefferentCount( const CObject & secondObject ) const {
    unsigned int result = 0,
        size;
    if ( data_.size() > secondObject.data_.size() ) {
        size = secondObject.data_.size();
        result += data_.size() - size;
    } else {
        size = data_.size();
        result += secondObject.data_.size() - size;
    }
    for ( unsigned int i = 0; i < size; i++ ) {
        if ( data_ [ i ] != secondObject.data_ [ i ] )
            result++;
    }
    return result;
}

std::vector<unsigned char>& Claster::CObject::getDataBuffer() {
    return data_;
}

std::string & Claster::CObject::getName() {
    return name_;
}

void Claster::CObject::setNextObject( CObject & object ) {
    CObject * head = this;
    while ( head->nextObject_ != nullptr )
        head = head->nextObject_;
    head->nextObject_ = &object;
}

std::string Claster::CObject::getClasters() {
    std::string result;

    if ( nextObject_ != nullptr ) {
        if ( isNextLock_ )
            result = ';';
        else
            result = '+';
        result += nextObject_->getClasters();

        
    }
    result = name_ + result;
    return result;
}

void Claster::CObject::setStateNext( bool state ) {
    isNextLock_ = state;
}

unsigned int Claster::CObject::deleteMaxEdge( unsigned int maxSize = 0 ) {
    if ( nextObject_ != nullptr ) {
        if ( isNextLock_ )
            return nextObject_->deleteMaxEdge( maxSize );
        else {
            unsigned int nextDef = nextObject_->getDefferentCount( *this );
            if ( nextDef > maxSize )
                maxSize = nextDef;
            maxSize = nextObject_->deleteMaxEdge( maxSize );
            if ( maxSize == nextDef ) {
                isNextLock_ = true;
                return UINT32_MAX;
            }
        }

    } else
        return maxSize;
}

void Claster::CObject::viewInfo() {
    std::cout << name_ << ": ";
    for ( auto iter = data_.begin(); iter != data_.end(); ++iter )
        std::cout << unsigned int (*iter) << ' ';
    std::cout << std::endl;
}

Claster::CObject::CObject( std::string name, const unsigned char * data, unsigned int sizeData ) {
    data_.reserve( sizeData );
    data_.insert( data_.end(), data, data + sizeData );
    name_ = name;
}

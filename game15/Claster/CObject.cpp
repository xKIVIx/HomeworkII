#include "CObject.h"
#include <iostream>

// Constructors/Destructors
//  


/**
* Set data
* @param data the new data
* @param size the size new data
*/

Claster::CObject::CObject()
{
    data_ = nullptr;
    dataSize_ = 0;
}

Claster::CObject::~CObject()
{
    if ( data_ != nullptr )
        delete [] data_;
}

Claster::CObject::CObject( const CObject & second )
{
    *this = second;
}

unsigned int Claster::CObject::getDefferentCount( CObject secondObject ) const
{
    unsigned int result = 0,
        size;
    if ( dataSize_ > secondObject.dataSize_ )
        size = secondObject.dataSize_;
    else
        size = dataSize_;
    for ( unsigned int i = 0; i < size; i++ )
    {
        if ( data_ [ i ] != secondObject.data_ [ i ] )
            result++;
    }
    return result;
}

inline void Claster::CObject::setData( unsigned char * data, unsigned int size )
{
    if ( data_ != nullptr )
    {
        delete [] data_;
        data_ = nullptr;
        dataSize_ = 0;
    }

    if ( data != nullptr )
    {
        data_ = new unsigned char [ size ];
        memcpy( data_, data, sizeof( char ) * size );
        dataSize_ = size;
    }
}

std::string Claster::CObject::getName()
{
    return name_;
}

void Claster::CObject::setNextObject( CObject & object )
{
    CObject * head = this;
    while ( head->nextObject_ != nullptr )
        head = head->nextObject_;
    head->nextObject_ = &object;
}

std::string Claster::CObject::getClasters()
{
    std::string result;

    if ( nextObject_ != nullptr )
    {
        result = nextObject_->getClasters();

        if ( isNextLock_ )
            result += ';';
        else
            result += '+';
    }
    result += name_;
    return result;
}

void Claster::CObject::setStateNext( bool state )
{
    isNextLock_ = state;
}

unsigned int Claster::CObject::deleteMaxEdge( unsigned int maxSize = 0 )
{
    
    if ( nextObject_ != nullptr )
    {
        if ( !isNextLock_ )
        {
            unsigned int size = nextObject_->getDefferentCount( *this );
            if ( size > maxSize )
                maxSize = size;
            maxSize = nextObject_->deleteMaxEdge( maxSize );
            if ( size == maxSize )
            {
                isNextLock_ = true;
                maxSize = UINT32_MAX;
            }
        }
    }
    return nextObject_->deleteMaxEdge( maxSize );
}

void Claster::CObject::viewInfo()
{
    std::cout << name_ << ": ";
    for ( unsigned int i = 0; i < dataSize_; i++ )
        std::cout << int( data_ [ i ] ) << ' ';
    std::cout << std::endl;
}

Claster::CObject::CObject( std::string name, unsigned char * data, unsigned int sizeData )
{
    setData( data, sizeData );
    name_.swap (name);
}

Claster::CObject & Claster::CObject::operator= ( const Claster::CObject & second )
{
    if ( this == &second )
        return *this;

    setData( second.data_, second.dataSize_ );
    name_ = second.name_;
    return *this;
}

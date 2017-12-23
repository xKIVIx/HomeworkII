#pragma once
#include <string>

#include "../COpenCl.h"
typedef unsigned char byte;
namespace Tests {
    namespace BitStorage {
        namespace Exceptions {
            const std::string uncorrectCompare = "Uncorrect compare bits storages\n",
                uncorrectSize = "Uncorrect count buffers\n";
        }
        struct SBitAdress {
            size_t bufferId_ = 0;
            char shift_ = 0;
        };
        template < typename TBufferType >
        class CBitStorage {
        public:

            

            /*
             * Adds the last bit of the parameter in the buffer.
             * @param bit - byte the last bit, which is added to the buffer.
             */
            void addBit( const byte byte );

            /*
             * TODO
             * Adds an array of bits. Bits are taken from the end.
             * @param bytes - the byte array in which stored bits to add.
             * @param countBytes - count bytes in array.
             * @param countBits - the number of bits to be added.
             */
            void addBits( const byte * bytes,
                          const size_t countBytes,
                          const size_t countBits );

            /*
             * To get a bit.
             * @param bitAdress - struct where storage bit adress
             * @return - bytes where the last bit is requested. If there is a handling error, the second bit is 1.
             */
            byte getBit( const SBitAdress & bitAdress ) const;

            /*
             * To get a bits.
             * @param buffer - buffer where result will be written.
             * @param sizeBuffer - size of result buffer
             * @return - count bits, which written.
             */
            size_t getBits( TBufferType * buffer,
                            const size_t sizeBuffer ) const;

            /*
             * To get count byte buffers for store bits.
             * @return - count buffers.
             */
            size_t getCountByffers() const;

            /*
             * To get count one`s bits.
             * @return - count one`s bits.
             */
            size_t getCountOnes() const;

            /*
             * To get skip state.
             * @return - skip state.
             */
            bool getSkipState() const;

            /*
             * To set skip state.
             * @isSkip - skip state.
             */
            void setSkipState( bool isSkip );


            /*
             * To set a bit.
             * @param bitAdress - struct where storage bit adress.
             * @param bit - byte the last bit, which is new.
             * @return - true if the bit was successfully changed, otherwise returns false
             */
            bool setBit( const SBitAdress & bitAdress,
                         const byte bit );

            /*
             * To set a bits.
             * @param bitAdress - struct where storage bit adress.
             * @param bit - byte the last bit, which is new.
             * @return - true if the bit was successfully changed, otherwise returns false
             */
            void setBits( const TBufferType * buffers,
                          const size_t  countBuffers );

            /*
             * It sets a new count buffers.
             * @param newSize - count byte buffers.
             * @return - true if resize is successfull
             */
            bool resize( const size_t newSize );

            CBitStorage();

            CBitStorage( const size_t size );

            CBitStorage( const CBitStorage & second );

            ~CBitStorage();

            CBitStorage & operator= ( const CBitStorage & second ) {
                if ( &second == this )
                    return *this;
                delete [] buffers_;
                buffers_ = new TBufferType [ second.countBuffers_ ] { 0 };
                memcpy( buffers_, second.buffers_, second.countBuffers_ * sizeof( TBufferType ) );
                countBuffers_ = second.countBuffers_;
                countOnes_ = second.countOnes_;
                dataSize_ = second.dataSize_;
                return *this;
            }

        private:
            SBitAdress lastBit_;

            const size_t sizeBuffer_ = sizeof( TBufferType ) * 8;

            size_t countBuffers_ = 0,
                countOnes_ = 0;

            TBufferType *buffers_ = nullptr;

            void recountOnes();

            bool isSkip_ = false;

        };

        /*
        * Adds the last bit of the parameter in the buffer.
        * @param bit - byte the last bit, which is added to the buffer.
        */
        template<typename TBufferType>
        inline void CBitStorage<TBufferType>::addBit( const byte byte ) {

            // allocating memory if necessary.
            if ( lastBit_.shift_ == -1 ) {
                lastBit_.bufferId_++;
                resize( countBuffers_ + 1 );
                lastBit_.shift_ = sizeBuffer_ - 1;
            }

            // extract and align the bit to be added.
            TBufferType writeByte = TBufferType( byte & 1 );
            if ( writeByte == 1 )
                countOnes_++;
            writeByte <<= lastBit_.shift_;
            // write bit
            buffers_ [ lastBit_.bufferId_ ] |= writeByte;
            lastBit_.shift_--;
        }

        /*
        * TODO
        * Adds an array of bits. Bits are taken from the end.
        * @param bytes - the byte array in which stored bits to add.
        * @param countBytes - count bytes in array.
        * @param countBits - the number of bits to be added.
        */
        template<typename TBufferType>
        inline void CBitStorage<TBufferType>::addBits( const byte * bytes, const size_t countBytes, const size_t countBits ) {
            // allocating memory if necessary.
            // calculate the necessary shift.
        }

        /*
        * To get a bit.
        * @param bitAdress - struct where storage bit adress
        * @return - bytes where the last bit is requested. If there is a handling error, the second bit is 1.
        */
        template<typename TBufferType>
        inline byte CBitStorage<TBufferType>::getBit( const SBitAdress & bitAdress ) const {
            // check out of data
            if ( bitAdress.bufferId_ >= countBuffers_ )
                return 3;

            return ( buffers_ [ bitAdress.bufferId_ ] >> bitAdress.shift_ ) & 1;
        }

        /*
        * To get a bits.
        * @param buffer - buffer where result will be written.
        * @param sizeBuffer - size of result buffer
        * @return - count bits, which written.
        */
        template<typename TBufferType>
        inline size_t CBitStorage<TBufferType>::getBits( TBufferType * buffer, const size_t sizeBuffer ) const {
            // define  count bits read
            size_t sizeRead;
            if ( sizeBuffer > countBuffers_ )
                sizeRead = countBuffers_;
            else
                sizeRead = sizeBuffer;

            memcpy( buffer, buffers_, sizeRead * sizeof( TBufferType ) );
            return sizeRead;
        }

        /*
        * To get count byte buffers for store bits.
        * @return - count buffers.
        */
        template<typename TBufferType>
        inline size_t CBitStorage<TBufferType>::getCountByffers() const {
            return countBuffers_;
        }

        template<typename TBufferType>
        inline size_t CBitStorage<TBufferType>::getCountOnes() const {
            return countOnes_;
        }

        template<typename TBufferType>
        inline bool CBitStorage<TBufferType>::getSkipState() const {
            return isSkip_;
        }

        template<typename TBufferType>
        inline void CBitStorage<TBufferType>::setSkipState( bool isSkip ) {
            isSkip_ = isSkip;
        }

        /*
        * To set a bit.
        * @param bitAdress - struct where storage bit adress.
        * @param bit - byte the last bit, which is new.
        * @return - true if the bit was successfully changed, otherwise returns false
        */
        template<typename TBufferType>
        inline bool CBitStorage<TBufferType>::setBit( const SBitAdress & bitAdress, const byte bit ) {
            if ( ( bitAdress.bufferId_ >= countBuffers_ ) ||
                ( bitAdress.shift_ >= sizeBuffer_ ) ) {
                return false;
            }
            TBufferType leftMask = ( ~0 ) << ( bitAdress.shift_ + 1 ),
                rightMask = ( ~0 ) >> ( sizeBuffer_ - bitAdress.shift_ );

            TBufferType newData = ( buffers_ [ bitAdress.bufferId ] & leftMask ) |
                ( buffers_ [ bitAdress.bufferId ] & rightMask ) |
                ( TBufferType( bit ) << bitAdress.shift_ );

            buffers_ [ bitAdress.bufferId ] = newData;
            recountOnes();
            return true;
        }

        /*
        * To set a bits.
        * @param bitAdress - struct where storage bit adress.
        * @param bit - byte the last bit, which is new.
        * @return - true if the bit was successfully changed, otherwise returns false
        */
        template<typename TBufferType>
        inline void CBitStorage<TBufferType>::setBits( const TBufferType * buffers, const size_t countBuffers ) {
            if ( buffers_ == nullptr )
                delete [] buffers_;
            countBuffers_ = countBuffers;
            buffers_ = new TBufferType [ countBuffers + 1 ];
            memcpy( buffers_, buffers, countBuffers * sizeof( TBufferType ) );
            lastBit_.bufferId_ = countBuffers;
            lastBit_.shift_ = -1;
            recountOnes();
        }

        /*
        * It sets a new count buffers.
        * @param newSize - count byte buffers.
        * @return - true if resize is successfull
        */
        template<typename TBufferType>
        inline bool CBitStorage<TBufferType>::resize( const size_t newSize ) {
            if ( newSize == 0 )
                return false;
            if ( newSize != countBuffers_ ) {
                // init new buffers
                TBufferType * newBuffers = new TBufferType [ newSize ] { 0 };
                if ( newSize > countBuffers_ )
                    memcpy( newBuffers, buffers_, countBuffers_ * sizeof( TBufferType ) );
                else {
                    memcpy( newBuffers, buffers_, newSize * sizeof( TBufferType ) );
                    lastBit_.bufferId_ = newSize - 1;
                    lastBit_.shift_ = sizeBuffer_ - 1;
                }
                // save new buffers
                delete [] buffers_;
                buffers_ = newBuffers;
                countBuffers_ = newSize;
                recountOnes();
            }
            return true;
        }
        template<typename TBufferType>
        inline CBitStorage<TBufferType>::CBitStorage() {
            lastBit_.shift_ = sizeBuffer_ - 1;
            countBuffers_ = 1;
            buffers_ = new TBufferType( 0 );
        }
        template<typename TBufferType>
        inline CBitStorage<TBufferType>::CBitStorage( const size_t size ) {
            lastBit_.shift_ = sizeBuffer_ - 1;
            countBuffers_ = size;
            buffers_ = new TBufferType [ countBuffers_ ] { 0 };
        }
        template<typename TBufferType>
        inline CBitStorage<TBufferType>::CBitStorage( const CBitStorage<TBufferType> & second ) {
            buffers_ = new TBufferType [ second.countBuffers_ ];
            memcpy( buffers_,
                    second.buffers_,
                    second.countBuffers_ * sizeof( TBufferType ) );
            countBuffers_ = second.countBuffers_;
            lastBit_ = second.lastBit_;
            countOnes_ = second.countOnes_;
        }
        template<typename TBufferType>
        inline CBitStorage<TBufferType>::~CBitStorage() {
            delete [] buffers_;
        }
        template<typename TBufferType>
        inline void CBitStorage<TBufferType>::recountOnes() {
            if ( sizeBuffer_ / 8 == 1 )
                countOnes_ = COpenCl::getInstance().computeTrueBits( ( byte * ) buffers_,
                                                                     countBuffers_ );
            else
                countOnes_ = COpenCl::getInstance().computeTrueBits( ( byte * ) buffers_,
                                                                     countBuffers_ * ( sizeBuffer_ / 8 ) );
        }
}
}

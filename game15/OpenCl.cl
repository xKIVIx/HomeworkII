__kernel void countOnes ( __global __read_only unsigned char * data, 
                          __global             unsigned int  * result ){
    unsigned int byteId = get_global_id( 0 ),
                 shift = get_global_id( 1 );
    unsigned char bit = (data[ byteId ] >> shift) & 1;
    if ( bit != 0 )
        atomic_inc( result );        
}

__kernel void countDiff ( __global __read_only unsigned char * data1,
                          __global __read_only unsigned char * data2,
						  __global             unsigned char * writeBuffer ){
    unsigned int dataId1 = get_global_id( 0 ),
                 dataId2 = get_global_id( 1 ),
                 bufferId = get_global_id( 2 ),
                 countBuffers = get_global_size( 2 );
    unsigned int currWritePosition = dataId1 * get_global_size( 1 ) + dataId2;
    writeBuffer[ currWritePosition * countBuffers + bufferId] = 
        data1[ dataId1*countBuffers + bufferId] ^ 
        data2[ dataId2*countBuffers + bufferId];
}

__kernel void decomposeData ( __global __read_only unsigned char * data,
                              __global             unsigned int  * writePos,
                              __global             unsigned char * result ){
    unsigned int bufferId = get_global_id( 0 ),
                 shift = get_global_id( 1 );
    unsigned char tmp = data[ bufferId ]&( 1 << shift );
    if ( tmp > 0 ) {
        result[ get_global_size( 0 ) * atomic_inc( writePos ) + bufferId ] = tmp;
    }
}
void m_memcpy ( __global void * dst, __global void * src, unsigned int size){
    for ( unsigned int k = 0; k  < size; k++){
        ((__global char *)dst)[k] = ((__global char *)src)[k];
    }
}
__kernel void computeTest ( __global __read_only unsigned char * data,
                                                 unsigned int    dataSize,
                            __global __read_only unsigned char * secondData,
                            __global             unsigned char * result,
                            __global             unsigned int  * writePos ) {
                                
    const unsigned int secondDataPos = get_global_id (0) * dataSize;
    for ( unsigned int i = 0; i < dataSize; i++ ) {
        if ( ( secondData[secondDataPos + i] & data[i] ) != 0 ) {
            unsigned int pos = atomic_inc( writePos ) * dataSize;
            m_memcpy( &result[pos], &secondData[secondDataPos], dataSize );
            return;
        }
    }
    
    for ( unsigned int i = 0; i < dataSize; i++ ) {
        for ( unsigned char shift = 0; shift < 8; shift++){
            if ( ( data[i] & ( 1 << shift ) ) != 0 ) {
                unsigned int pos = atomic_inc( writePos ) * dataSize;
                m_memcpy( &result[pos], &secondData[secondDataPos], dataSize );
                result[pos+i] |= ( data[i] & ( 1<<shift ) );
            } 
        }
    }
}
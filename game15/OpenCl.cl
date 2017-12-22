__kernel void countOnes ( __global __read_only unsigned char * data, 
                          __global unsigned    int           * result ){
    unsigned int byteId = get_global_id( 0 ),
                 shift = get_global_id( 1 );
    unsigned char bit = (data[ byteId ] >> shift) & 1;
    if ( bit != 0 )
        atomic_inc( result );        
}

__kernel void countDiff ( __global __read_only unsigned char * data1,
                          __global __read_only unsigned char * data2,
						  __global __read_only unsigned int  * countBuffers,
						  __global             unsigned int  * writePosition,
						  __global             unsigned char * writeBuffer ){
    unsigned int dataId1 = get_global_id( 0 ),
                 dataId2 = get_global_id( 1 );
    currWritePosition = atomic_inc ( writePosition );
    for ( unsigned int i = 0; i < *countBuffers; i++) {
        writeBuffer[ currWritePosition * (*countBuffers) + i] = 
            ~( data1[ dataId1*(*countBuffers) + i] ^ 
               data2[ dataId2*(*countBuffers) + i] )
    }
}

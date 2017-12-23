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

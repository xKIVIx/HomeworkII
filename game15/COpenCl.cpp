#include "COpenCl.h"
#include <iostream>
#include <fstream>

COpenCl * COpenCl::instance_ = nullptr;

void chek( const cl_int  stat ) {
    if ( stat != CL_SUCCESS ) {
        std::cout << "Fail: "<< stat << std::endl;
        throw stat;
    }
}
size_t COpenCl::computeTrueBits( const byte * data,
                                 const size_t dataSize ) {
    // create buffers
    size_t result = 0;

    CMemObject dataBuffer( dataSize, CL_MEM_READ_ONLY );
    CMemObject resultBuffer( sizeof( size_t ), 
                             CL_MEM_READ_WRITE );

    // load data in buffer
    dataBuffer.loadData( data, dataSize );
    resultBuffer.loadData( ( byte* ) &result, 
                           sizeof( result ) );
    // init karnel
    CKernel kernel( "countOnes" );

    // set function parametrs
    kernel.bindParametr( dataBuffer, 0 );
    kernel.bindParametr( resultBuffer, 1 );

    // complite compute
    cl_uint workSize [] = { dataSize,
        // bit size of char
        8 };
    kernel.complite( workSize, 2 );

    // get result
    byte * buffer;
    resultBuffer.getData( &buffer );
    result = *( ( size_t * ) buffer );
    delete [] buffer;
    return result;
}
size_t COpenCl::computeDiff( const byte * data1,
                             const byte * data2,
                             const size_t sizeData1,
                             const size_t sizeData2,
                             const size_t countBuffers,
                             byte ** result ) {

    // init data buffers
    size_t sizeBuffer1 = sizeData1 * countBuffers;
    CMemObject dataBuffer1( sizeBuffer1, CL_MEM_READ_ONLY );

    size_t sizeBuffer2 = sizeData2 * countBuffers;
    CMemObject dataBuffer2( sizeBuffer2, CL_MEM_READ_ONLY );

    // init result buffer
    CMemObject resultBuffer( sizeBuffer1 * sizeData2, 
                             CL_MEM_WRITE_ONLY );

    // load data in buffer
    dataBuffer1.loadData( data1, sizeBuffer1 );
    dataBuffer2.loadData( data2, sizeBuffer2 );

    // init karnel
    CKernel kernel( "countDiff" );
    kernel.bindParametr( dataBuffer1, 0 );
    kernel.bindParametr( dataBuffer2, 1 );
    kernel.bindParametr( resultBuffer, 2 );

    size_t workSizes [] = { sizeData1, sizeData2, countBuffers };
    kernel.complite( workSizes, 3 );

    return resultBuffer.getData( result ) / countBuffers;
}
size_t COpenCl::computeTests( const byte * data, 
                              const size_t * countOnes, 
                              const size_t countRows, 
                              const size_t rowSize, 
                              byte ** result ) {
    CMemObject dataBuffer( rowSize , CL_MEM_READ_ONLY );

    CMemObject resultBuffers [ 2 ] { CMemObject( rowSize,CL_MEM_READ_WRITE ),
                                     CMemObject( countOnes [ 0 ] * rowSize,CL_MEM_READ_WRITE ) };
    resultBuffers [ 0 ].fill( 0 );

    CMemObject resultSizeBuffer( sizeof( size_t ), CL_MEM_READ_WRITE );

    size_t resultSize = 1;

    CKernel kernel( "computeTest" );
    kernel.bindParametr( dataBuffer, 0 );
    kernel.bindParametr( rowSize, 1 );
    for ( size_t i = 0; i < countRows; i++ ) {

        dataBuffer.loadData( &data [ i * rowSize ], rowSize );
        resultSizeBuffer.fill( 0 );
        resultBuffers [ ( i + 1 ) % 2 ].resize( resultSize * countOnes [ i ] * rowSize );
        kernel.bindParametr( resultBuffers [ i % 2 ], 2 );
        kernel.bindParametr( resultBuffers [ ( i + 1 ) % 2 ], 3 );
        kernel.bindParametr( resultSizeBuffer, 4 );
        kernel.complite( &resultSize, 1 );
        clFinish( commandQueue_ );
        size_t * tmp;
        resultSizeBuffer.getData( (byte **)&tmp );
        resultSize = *tmp;
        delete tmp;
    }
    resultBuffers [ countRows % 2 ].getData( result );
    return resultSize / rowSize;
}
size_t COpenCl::decomposeData( const byte * data,
                               const size_t countOnes,
                               const size_t rowSize,
                               byte ** result ) {
    // load data
    CMemObject dataMemObject( rowSize, CL_MEM_READ_ONLY );
    dataMemObject.loadData( data, rowSize);

    // decopose first row
    CMemObject writePos( sizeof( size_t ), CL_MEM_READ_WRITE );
    writePos.fill( 0 );

    size_t sizeResult = countOnes*rowSize;
    CMemObject resultBuffer( sizeResult, CL_MEM_WRITE_ONLY );
    resultBuffer.fill( 0 );
    // init karnel    
    CKernel kernelDecompose( "decomposeData" );

    // set function parametrs
    kernelDecompose.bindParametr( dataMemObject, 0 );
    kernelDecompose.bindParametr( writePos, 1 );
    kernelDecompose.bindParametr( resultBuffer, 2 );
    size_t workSize [] = { rowSize, 8 };
    kernelDecompose.complite( workSize, 2 );
    clFinish( commandQueue_ );
    *result = new byte [ sizeResult ];
    resultBuffer.loadData( *result, sizeResult );
    return sizeResult;
}
COpenCl & COpenCl::getInstance() {
    if ( instance_ == nullptr )
        throw "OpenCLNotInstal";
    return *instance_;
}
void COpenCl::init() {
    if ( instance_ == nullptr ) {
        instance_ = new COpenCl();
    }
}
void COpenCl::release() {
    if ( instance_ != nullptr ) {
        delete instance_;
        instance_ = nullptr;
    }
}
COpenCl::COpenCl() {
    std::cout << "-------Init OpenCl-------\n";

    // init platform 
    std::cout << "Init platform\n";
    cl_uint countPlatforms = 0;
    chek( clGetPlatformIDs( 0, 
                            nullptr, 
                            &countPlatforms ) );
    cl_platform_id * platfroms = new cl_platform_id [ countPlatforms ];
    chek( clGetPlatformIDs( countPlatforms, 
                            platfroms, 
                            &countPlatforms ) );
    cl_platform_id selectedPlatform = platfroms [ countPlatforms - 1 ];
    char name [ 256 ];
    cl_uint nameSize = 0;
    chek( clGetPlatformInfo( selectedPlatform, 
                             CL_PLATFORM_NAME, 
                             256, 
                             name, 
                             &nameSize ) );
    std::cout << "Platform: "<< name << std::endl;

    // init device
    std::cout << "Init device"<< std::endl;
    cl_uint countDevices = 0;
    chek( clGetDeviceIDs( selectedPlatform, 
                          CL_DEVICE_TYPE_GPU, 
                          0, 
                          nullptr, 
                          &countDevices) );
    cl_device_id * devices = new cl_device_id [ countDevices ];
    chek( clGetDeviceIDs( selectedPlatform, 
                          CL_DEVICE_TYPE_GPU, 
                          countDevices, 
                          devices, 
                          &countDevices) );
    cl_device_id selectedDevice = devices [ countDevices - 1 ];
    chek( clGetDeviceInfo( selectedDevice, 
                           CL_DEVICE_NAME, 
                           256, 
                           name, 
                           &nameSize ) );
    std::cout << "Device: " << name << std::endl;

    // create context
    std::cout << "Create context\n";
    cl_int err = 0;
    context_ = clCreateContext( NULL,
                                1, 
                                &selectedDevice, 
                                nullptr, 
                                nullptr, 
                                &err );
    chek( err );

    // create command queue
    std::cout << "Create command queue\n";
    commandQueue_ = clCreateCommandQueue( context_,
                                          selectedDevice, 
                                          0, 
                                          &err );
    chek( err );

    
    // read .cl file
    std::cout << "Read .cl file\n";
    std::fstream file( "OpenCl.cl" );
    if ( !file.is_open() ) {
        std::cout << "Can`t open OpenCl.cl\n";
        throw "NotFoundOpenclProgram";
    }
    std::string script;
    while ( !file.eof() ) {
        char tmp [ 256 ];
        file.getline( tmp, 256 );
        script += tmp;
        script += '\n';
    }

    // compile prgram
    std::cout << "Compile program\n";
    const char * tmp = script.c_str();
    size_t sizeScript =  script.size();
    program_ = clCreateProgramWithSource( context_, 
                                          1, 
                                          &tmp, 
                                          &sizeScript, 
                                          &err );
    chek( err );
    try {
        chek( clBuildProgram( program_,
              1,
              &selectedDevice,
              NULL,
              NULL,
              NULL ) );
    }
    catch ( int err ) {
        size_t endLog = 0;
        clGetProgramBuildInfo( program_,
                               selectedDevice,
                               CL_PROGRAM_BUILD_LOG,
                               1000,
                               nullptr,
                               &endLog );
        char * log = new char [ endLog ];
        clGetProgramBuildInfo( program_,
                               selectedDevice,
                               CL_PROGRAM_BUILD_LOG,
                               endLog,
                               log,
                               &endLog );
        log [ endLog-1 ] = '\0';
        std::cout << log;
    }
    std::cout << "----------OpenCl is success init---------\n";
}

COpenCl::~COpenCl() {
    clReleaseProgram( program_ );
    clReleaseCommandQueue( commandQueue_ );
    clReleaseContext( context_ );
}

COpenCl::CMemObject::CMemObject( const size_t size , 
                                 const cl_mem_flags flag) {
    flag_ = flag;
    resize( size );
}

COpenCl::CMemObject::CMemObject( const CMemObject & sec ) {
    *this = sec;
}

COpenCl::CMemObject & COpenCl::CMemObject::operator=( const CMemObject & sec ) {
    if ( this == &sec ) {
        return *this;
    }
    flag_ = sec.flag_;
    resize( sec.size_ );

    chek( clEnqueueCopyBuffer( COpenCl::getInstance().commandQueue_,
                               sec.mem_,
                               mem_,
                               0,
                               0,
                               size_,
                               NULL,
                               nullptr,
                               nullptr ) );
    return *this;
}

COpenCl::CMemObject::~CMemObject() {
    clReleaseMemObject( mem_ );
}

void COpenCl::CMemObject::loadData( const byte * data,
                                    const size_t size ) {
    if ( size_ != size )
        throw "LoadDataError";
    chek( clEnqueueWriteBuffer( COpenCl::getInstance().commandQueue_,
          mem_,
          CL_TRUE,
          0,
          size * sizeof( byte ),
          data,
          0,
          NULL,
          NULL ) );
}

size_t COpenCl::CMemObject::getData( byte ** buffer ) {
    *buffer = new byte [ size_ ];
    cl_uint err;
    err = clEnqueueReadBuffer( COpenCl::getInstance().commandQueue_,
                               mem_,
                               CL_TRUE,
                               0,
                               size_ * sizeof( byte ),
                               *buffer,
                               0,
                               NULL,
                               NULL );
    chek( err );
    return size_;
}

void COpenCl::CMemObject::resize( const size_t newSize ) {
    if ( mem_ != nullptr ) {
        clReleaseMemObject( mem_ );
    }
    cl_int err;
    size_ = newSize;
    mem_ = clCreateBuffer( COpenCl::getInstance().context_,
                           flag_,
                           size_ * sizeof( byte ),
                           NULL,
                           &err );
    chek( err );
}

void COpenCl::CMemObject::fill( const byte fillByte ) {
    byte * fill = new byte [ size_ ] { fillByte };
    loadData( fill, size_ );
    delete fill;
}

COpenCl::CKernel::CKernel( const char * name ) {
    cl_int err = 0;
    kernel_ = clCreateKernel( COpenCl::getInstance().program_, name, &err );
    chek( err );
}

COpenCl::CKernel::~CKernel() {
    clReleaseKernel( kernel_ );
}

void COpenCl::CKernel::bindParametr( const size_t paramData, const size_t paramId ) {
    chek( clSetKernelArg( kernel_, paramId, sizeof( size_t ), &paramData ) );
}

void COpenCl::CKernel::bindParametr( const CMemObject & memObject, 
                                      const size_t paramId ) {
    chek( clSetKernelArg( kernel_, paramId, sizeof( cl_mem ), &memObject.mem_ ) );    
}

void COpenCl::CKernel::complite( const size_t * workSizes, 
                                 const size_t countWorkSizes ) {
    chek( clEnqueueNDRangeKernel( COpenCl::getInstance().commandQueue_,
          kernel_,
          countWorkSizes,
          NULL,
          workSizes,
          NULL,
          NULL,
          NULL,
          NULL ) );
}

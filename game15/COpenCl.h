#pragma once
#include "CL\cl.h"
#pragma comment(lib,"OpenCl.lib")
typedef unsigned char byte;
class COpenCl {
public:
    size_t computeTrueBits( const byte * data,
                            const size_t dataSize );
    size_t computeDiff( const byte * data1,
                        const byte * data2,
                        const size_t sizeData1,
                        const size_t sizeData2,
                        const size_t countBuffers,
                        byte ** result );
    size_t computeTests(const byte * data,
                         const size_t * countOnes,
                         const size_t countRows,
                         const size_t rowSize,
                         byte ** result);
    size_t decomposeData( const byte * data,
                          const size_t countOnes,
                          const size_t rowSize,
                          byte ** result );
    static COpenCl & getInstance();
    static void init();
    static void release();
private:
    class CKernel;
    class CMemObject {
    public:
        CMemObject( const size_t size, 
                    const cl_mem_flags flag );
        CMemObject( const CMemObject & sec );
        CMemObject & operator= ( const CMemObject & sec );
        ~CMemObject();
        void loadData( const byte * data, 
                       const size_t size );
        size_t getData( byte ** buffer );
        void resize( const size_t newSize );
        void fill( const byte fillByte );
        friend CKernel;
    private:
        cl_mem mem_ = nullptr;
        size_t size_ = 0;
        cl_mem_flags flag_;
    };

    class CKernel {
    public:
        CKernel( const char * name );
        ~CKernel();
        void bindParametr( const size_t paramData,
                           const size_t paramId );
        void bindParametr( const CMemObject & memObject,
                           const size_t paramId );
        void complite( const size_t * workSizes, 
                       const size_t countWorkSizes );
    private:
        cl_kernel  kernel_;

    };

    cl_context context_;
    cl_command_queue commandQueue_;
    cl_program program_;
    static COpenCl * instance_;
    COpenCl();
    ~COpenCl();
};

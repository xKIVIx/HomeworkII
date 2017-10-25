
#ifndef COBJECT_H
#define COBJECT_H

#include <string>
#include <vector>



namespace Claster
{

    class CObject
    {
    public:

        // Constructors/Destructors
        //  


        /**
         * Empty Constructor
         */
        CObject();


        /**
         * Empty Destructor
         */
        ~CObject();

        /**
         * Copy
         */
        CObject( const CObject & second );



        /**
         * different between two object
         * @return unsigned int
         * @param  secondObject second object to calculate defferent count
         */
        unsigned int getDefferentCount( CObject secondObject ) const;


        /**
         * Set data
         * @param data the new data
         * @param size the size new data
         */
        void setData( unsigned char * data, unsigned int size );

        std::string getName();

        void setNextObject( CObject & object );

        std::string getClasters();

        void setStateNext( bool state );

        unsigned int deleteMaxEdge( unsigned int maxSize = 0 );

        void viewInfo();

        /**
         * object data
         * @param  data
         * @param  sizeData size input data
         */
        CObject( std::string name, unsigned char * data, unsigned int sizeData );


        CObject& operator= ( const CObject & second );

    protected:

        // Private attributes
        //  
        std::string name_;
        unsigned char * data_;
        unsigned int dataSize_;

        CObject * nextObject_;
        bool isNextLock_ = false;
    };
} // end of package namespace

#endif // COBJECT_H

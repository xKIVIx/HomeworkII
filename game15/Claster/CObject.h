
#ifndef COBJECT_H
#define COBJECT_H

#include <string>
#include <vector>



namespace Claster {

    class CObject {
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
         * different between two object
         * @return unsigned int
         * @param  secondObject second object to calculate defferent count
         */
        unsigned int getDefferentCount( const CObject & secondObject ) const;

        std::vector <unsigned char> & getDataBuffer();

        std::string & getName();

        void setNextObject( CObject & object );

        std::string getClasters();

        void setStateNext( bool state );

        unsigned int deleteMaxEdge( unsigned int maxSize);

        void viewInfo();

        /**
         * object data
         * @param  data
         * @param  sizeData size input data
         */
        CObject( const std::string & name, 
                 const unsigned char * data, 
                 unsigned int sizeData );



    protected:

        // Private attributes
        //  
        std::string name_;
        std::vector <unsigned char> data_;

        CObject * nextObject_ = nullptr;
        bool isNextLock_ = false;
    };
} // end of package namespace

#endif // COBJECT_H

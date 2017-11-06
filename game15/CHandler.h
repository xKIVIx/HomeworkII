
#ifndef CHANDLER_H
#define CHANDLER_H

#include <string>
#include <vector>

#include "CHandler.h"

class CHandler {
public:

    // Constructors/Destructors
    //  


    /**
     * Empty Constructor
     */
    CHandler();

    /**
     * Empty Destructor
     */
    ~CHandler();



    /**
     * if haven`t handler return "Cann`t handle massage"
     * @return std::string
     * @param  message
     */
    virtual std::string handle( std::string message ) const = 0;

    void addNewHandler( CHandler * hendler );


protected:

    CHandler * m_nexthandler_ = nullptr;




};

#endif // CHANDLER_H

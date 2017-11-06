#include "CHandler.h"

#include <iostream>

// Constructors/Destructors
//  

CHandler::CHandler () 
{
}

CHandler::~CHandler() {
}


//  
// Methods
//  


// Accessor methods
//  


void CHandler::addNewHandler( CHandler * handler ) {
    CHandler * tmp = this;
    while ( tmp->m_nexthandler_ != nullptr )
        tmp = tmp->m_nexthandler_;
    tmp->m_nexthandler_ = handler;
}

std::string CHandler::nextHandler( std::string message ) const {
    if ( m_nexthandler_ != nullptr )
        return m_nexthandler_->handle( message );
    else {
        std::cout << "Haven`t \"" << message.back() << "\" handler\n";
        return "Haven`t handler\n";
    }
}


// Other methods
//  



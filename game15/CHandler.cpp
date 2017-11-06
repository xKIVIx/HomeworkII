#include "CHandler.h"

// Constructors/Destructors
//  

CHandler::CHandler () 
{
}

CHandler::~CHandler() {
    if ( m_nexthandler_ != nullptr )
        delete m_nexthandler_;
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


// Other methods
//  



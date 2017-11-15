#include "CGameHandler.h"
#include "CTreeElem.h"
#include <iostream>


namespace Game15 {
    std::string CGameHandler::handle( std::string message ) const {
        if ( message.back() == 'g' ) {
            std::cout << "Start game 15\n";
            message.pop_back();
            unsigned char field [ COUNT_CELL ] { 0 };
            unsigned int id = 0;
            unsigned char empyCellPos = UCHAR_MAX;
            const char separator [] = "+";
            char *str = new char [ message.length() ];
            memcpy( str, message.c_str(), sizeof( char ) * message.length() );

            char *ptr = nullptr;
            ptr = strtok( str, separator );
            while ( ptr ) {
                field [ id ] = atoi( ptr );
                if ( field [ id ] == UCHAR_MAX )
                    empyCellPos = id;
                id++;
                ptr = strtok( 0, separator );
            }
            delete [] str;
            if ( empyCellPos != UCHAR_MAX ) {
                Game15::CTreeElem test( field, empyCellPos );
                std::string answer = test.sreachWay();
                std::cout << "End find answer\n";
                return answer;
            }
            return "Cann`t find answer\n";
        } else
            return nextHandler( message );
    }
}



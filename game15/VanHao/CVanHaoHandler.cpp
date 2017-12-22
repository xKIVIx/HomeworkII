#include "CVanHaoHandler.h"
#include "CVanHao.h"

std::string VanHao::CVanHaoHandler::handle( std::string message ) const {
    if ( message.back() == 'v' ) {
        message.back() = '\0';
        std::tr1::regex rx("%7C");
        std::string replacement = "|";
        message = std::tr1::regex_replace(message, rx, replacement);
        rx.assign("%3E");
        replacement = ">";
        message = std::tr1::regex_replace(message, rx, replacement);
        const char separator [] = "+";
        char *str = new char [ message.length() ];
        memcpy( str, message.c_str(), sizeof( char ) * message.length() );
        std::string data [ 2 ];
        unsigned int i = 0;
        char *ptr = strtok( str, separator );
        while ( ptr ) {
            if ( i == 2 )
                return "Uncorrect data";
            data [ i ] = ptr;
            i++;
            ptr = strtok( 0, separator );
        }
        delete [] str;
        CVanHao handler;
        try {
            return handler.handle( data [ 0 ], data [ 1 ] );
        } catch ( const char * err ) {
            return err;
        }
    } else
        return nextHandler( message );

}

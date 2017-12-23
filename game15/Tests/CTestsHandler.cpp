#include <unordered_map>

#include "CTestsHandler.h"
#include "CCore.h"

std::string getWord( std::string & message ) {
    std::string result;
    while ( !message.empty() ) {
        if ( message.back() == '+' ) {
            message.pop_back();
            return result;
        }
        result += message.back();
        message.pop_back();
    }
    return result;
}

namespace Tests {
    CTestsHandler::CTestsHandler() {
        core_ = new CCore;
    }
    CTestsHandler::~CTestsHandler() {
        delete ( CCore * ) core_;
    }
    std::string CTestsHandler::handle( std::string & message ) {
        if ( message.back() != 't' )
            return nextHandler( message );
        message.pop_back();
        char operation = message.back();
        message.pop_back();
        switch ( operation ) {
            case 'n':
                return newData( message );
            case 'a':
                return addData( message );
            case 'd':
                return deleteData( message );
            default:
                return "Handle tests error! Unknown operation: " + operation;
                break;
        }
        return std::string();

    }
    std::string CTestsHandler::newData( std::string & message ) {
        std::unordered_map <std::string, size_t> setsMap;
        std::vector < std::list < BitStorage::CBitStorage < byte > > > data;
       
        while ( !message.empty() ) {
            std::string word = getWord( message );
            auto setIter = setsMap.find( word );
            size_t setId;
            if ( setIter != setsMap.end() ) {
                setId = setIter->second;
            } else {
                setId = data.size();
                setsMap.insert( std::pair<std::string, size_t>( word, setId ) );
                data.emplace_back();
            }
            word = getWord( message );
            data [ setId ].emplace_back();
            for ( auto iter = word.begin(); iter != word.end(); ++iter ) {
                if ( *iter == '1' )
                    data [ setId ].back().addBit( 1 );
                else
                    data [ setId ].back().addBit( 0 );
            }

        }
        ( ( CCore * ) core_ )->setData( data );
        ( ( CCore * ) core_ )->getTests();
        return std::string();
    }
    std::string CTestsHandler::addData( std::string & message ) {
        return std::string();
    }
    std::string CTestsHandler::deleteData( std::string & message ) {
        return std::string();
    }
}

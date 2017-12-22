#include "CVanHao.h"
#include <iostream>

#define VAN_HAO_ERROR_HAVENOT_ANSWER "Haven`t answer"
#define VAN_HAO_ERROR_UNCORRECT_DATA "Uncorrect data: "

#define VAN_HAO_NOT_REGEX "^~(?:~~)*(~*(?:\\([\\w\\W]+\\)|[\\w]+))$"
#define VAN_HAO_OPERATION_OR_REGEX "^((?:[\\w&|>~]|\\((?:[\\w~&|>]|\\([\\w\\W]+\\))+\\))+)\\|((?:[\\w&|>~]|\\((?:[\\w~&|>]|\\([\\w\\W]+\\))+\\))+)$"
#define VAN_HAO_OPERATION_AND_REGEX "^((?:[\\w&|>~]|\\((?:[\\w~&|>]|\\([\\w\\W]+\\))+\\))+)&((?:[\\w&|>~]|\\((?:[\\w~&|>]|\\([\\w\\W]+\\))+\\))+)$"
#define VAN_HAO_OPERATION_IMPL_REGEX "^((?:[\\w&|>~]|\\((?:[\\w~&|>]|\\([\\w\\W]+\\))+\\))+)>((?:[\\w&|>~]|\\((?:[\\w~&|>]|\\([\\w\\W]+\\))+\\))+)$"
#define VAN_HAO_IS_SCOB_REGEX "^\\(([\\w\\W]+)\\)$"
#define VAN_HAO_IS_WORD "^\\w+$"

std::string VanHao::CVanHao::handle( const std::string & leftData, const std::string & rightData ) {
    std::forward_list <std::string> leftDataVector,
        rightDataVector;
    std::cout << "Init data\n";
    leftDataVector = getDataList( leftData );
    rightDataVector = getDataList( rightData );
    viewDataVector( "Left data", leftDataVector );
    viewDataVector( "Right data", rightDataVector );
    std::string result;
    std::cout << "Start cheking\n";
    if ( !checkCorrect( leftDataVector ) ) {
        return VAN_HAO_ERROR_UNCORRECT_DATA + uncorrectData;
    }
    if ( !checkCorrect( rightDataVector ) ) {
        return VAN_HAO_ERROR_UNCORRECT_DATA + uncorrectData;
    }
    std::cout << "Start processing\n";
    try {
        result += processing( leftDataVector, rightDataVector );
        return result;
    } catch ( const char * err ) {
        if ( !uncorrectData.empty() ) {
            return err + uncorrectData;
        }
        return err;
    }
}

VanHao::CVanHao::CVanHao() {
    regexSimpleData_.assign( VAN_HAO_IS_WORD );
    regexIsScob_.assign( VAN_HAO_IS_SCOB_REGEX );
    regexNot_.assign( VAN_HAO_NOT_REGEX );
    regexAnd_.assign( VAN_HAO_OPERATION_AND_REGEX );
    regexOr_.assign( VAN_HAO_OPERATION_OR_REGEX );
    regexImpl_.assign( VAN_HAO_OPERATION_IMPL_REGEX );
}

bool VanHao::CVanHao::checkCorrect( const std::forward_list<std::string>& data ) {
    for ( auto i = data.begin(); i != data.end(); ++i ) {
        std::tr1::regex test( "\\([)&|>]|\\)\\(|[&>|][&>|]|[^\\w~&>|()]|[\w]\\(" );
        if ( std::tr1::regex_search( *i, test ) ) {
            uncorrectData = *i;
            return false;
        };
        int countScob = 0;
        for ( auto k = i->begin(); k != i->end(); ++k ) {
            if ( *k == '(' )
                countScob++;
            if ( *k == ')' )
                countScob--;
        }
        if ( countScob != 0 ) {
            uncorrectData = *i;
            return false;
        }
    }
    return true;
}

std::string VanHao::CVanHao::packData( const std::forward_list<std::string>& data1,
                                       const std::forward_list<std::string>& data2 ) {
    std::string result;
    for ( auto iter = data1.begin(); iter != data1.end(); ++iter ) {
        result += *iter;
        result += ',';
    }
    if ( !data1.empty() )
        result.pop_back();
    result += "->";
    for ( auto iter = data2.begin(); iter != data2.end(); ++iter ) {
        result += *iter;
        result += ',';
    }
    result.pop_back();
    return result;
}

std::forward_list<std::string> VanHao::CVanHao::getDataList( const std::string & data ) {
    const char separator [] = ",";
    char *str = new char [ data.length() + 1 ];
    memcpy( str, data.c_str(), sizeof( char ) * data.length() );
    str [ data.length() ] = '\0';
    char *ptr = strtok( str, separator );

    std::forward_list <std::string> result;
    while ( ptr ) {
        result.push_front( ptr );
        if ( result.front().size() == 0 )
            result.pop_front();
        ptr = strtok( 0, separator );
    }
    delete [] str;
    delete ptr;

    return result;
}

void VanHao::CVanHao::viewDataVector( const char * name, 
                                      const std::forward_list<std::string> & data ) {
    std::cout << name<<": ";
    for ( auto i = data.begin(); i != data.end(); ++i )
        std::cout << ( *i ) << ", ";
    std::cout << std::endl;
}

std::forward_list<std::string> VanHao::CVanHao::deleteRepeatElem( const std::forward_list<std::string>& data ) {
    std::forward_list<std::string> result;
    for ( auto dataIter = data.begin(); dataIter != data.end(); ++dataIter ) {
        unsigned int countEquel = 0;
        for ( auto resultIter = result.begin(); resultIter != result.end(); ++resultIter )
            if ( *dataIter == *resultIter )
                countEquel++;  
        if ( countEquel == 0 )
            result.push_front( *dataIter );
    }
    return result;
}

bool VanHao::CVanHao::isEqualData( const std::forward_list<std::string>& data1, 
                                   const std::forward_list<std::string>& data2 ) {
    unsigned int size1 = 0,
        size2 = 0;
    for ( auto dataIter1 = data1.begin(); dataIter1 != data1.end(); ++dataIter1 ) {
        bool isEqual = false;
        size1++;
        size2 = 0;
        for ( auto dataIter2 = data2.begin(); dataIter2 != data2.end(); ++dataIter2 ) {
            if ( ( *dataIter1 ) == ( *dataIter2 ) )
                isEqual = true;
            size2++;
        }
        if ( !isEqual )
            return false;
    }
    return size1 == size2;
}

std::string VanHao::CVanHao::processing( std::forward_list<std::string> leftData, 
                                         std::forward_list<std::string> rightData ) {
    clearFromScob( leftData );
    clearFromScob( rightData );
    std::string result = packData( leftData, rightData ) + '+';
    for ( ;;) {
        if ( deleteImplication( leftData ) || deleteImplication( rightData ) ) {
            leftData.swap( deleteRepeatElem( leftData ) );
            rightData.swap( deleteRepeatElem( rightData ) );
            result += packData( leftData, rightData ) + '+';
            continue;
        }
        if ( departNot( leftData, rightData ) ) {
            leftData = deleteRepeatElem( leftData );
            rightData = deleteRepeatElem( rightData );
            result += packData( leftData, rightData ) + '+';
            continue;
        }
        if ( deleteOperation( leftData, regexAnd_ ) || deleteOperation( rightData, regexOr_ ) ) {
            leftData = deleteRepeatElem( leftData );
            rightData = deleteRepeatElem( rightData );
            result += packData( leftData, rightData ) + '+';
            continue;
        }
        break;
    };

    if ( isSimpleData( leftData ) &&
         isSimpleData( rightData ) ) {
        if ( isEqualData( leftData, rightData ) ) {
            result.pop_back();
            return result;
        }
        throw VAN_HAO_ERROR_HAVENOT_ANSWER;
    }
    return result + nextProcessing( leftData, rightData );
}

bool VanHao::CVanHao::departNot( std::forward_list<std::string>& leftData, 
                                 std::forward_list<std::string>& rightData ) {
    bool isDepart = false,
        isMake = false;
    do {
        isDepart = false;
        std::tr1::cmatch res;
        std::forward_list<std::string> tmp;
        for ( auto iter = leftData.begin(); iter != leftData.end(); ++iter ) {
            if ( std::tr1::regex_search( iter->c_str(), res, regexNot_ ) ) {
                bool notDepart = false;
                std::string tmpData = res [ 1 ].str();
                if ( tmpData [ 0 ] == '~' ) {
                    tmpData = &tmpData.c_str() [ 1 ];
                    notDepart = true;
                }
                if ( std::tr1::regex_search( tmpData, regexSimpleData_ ) )
                    if ( notDepart )
                        tmp.push_front( tmpData );
                    else
                        rightData.push_front( tmpData );
                else
                    if ( deleteScob( tmpData ) )
                        if ( notDepart )
                            tmp.push_front( tmpData );
                        else
                            rightData.push_front( tmpData );
                    else {
                        tmp.push_front( *iter );
                        continue;
                    }

                    isDepart = true;
                    isMake = true;
            } else {
                tmp.push_front( *iter );
            }
        }
        leftData.swap( tmp );
        tmp.clear();
        for ( auto iter = rightData.begin(); iter != rightData.end(); ++iter ) {
            if ( std::tr1::regex_search( iter->c_str(), res, regexNot_ ) ) {
                bool notDepart = false;
                std::string tmpData = res [ 1 ].str();
                if ( tmpData [ 0 ] == '~' ) {
                    tmpData = &tmpData.c_str() [ 1 ];
                    notDepart = true;
                }
                if ( std::tr1::regex_search( tmpData, regexSimpleData_ ) )
                    if ( notDepart )
                        tmp.push_front( tmpData );
                    else
                        leftData.push_front( tmpData );
                else
                    if ( deleteScob( tmpData ) )
                        if ( notDepart )
                            tmp.push_front( tmpData );
                        else
                            leftData.push_front( tmpData );
                    else {
                        tmp.push_front( *iter );
                        continue;
                    }

                    isDepart = true;
                    isMake = true;
            } else {
                tmp.push_front( *iter );
            }
        }
        rightData.swap( tmp );
    } while ( isDepart );
    return isMake;
}

bool VanHao::CVanHao::deleteScob( std::string & data ) {
    unsigned int countScob = 1;
    std::string tmp;
    auto i = data.begin();
    if ( *i != '(' )
        return false;
    ++i;
    for ( ; i != data.end(); ++i ) {
        if ( ( countScob == 0 ) && ( *i != '\0' ) )
            return false;
        if ( *i == ')' ) {
            countScob--;
            if ( countScob == 0 )
                continue;
        }
        if ( *i == '(' )
            countScob++;
        tmp += *i;
    }
    if ( countScob != 0 ) {
        uncorrectData = data;
        throw VAN_HAO_ERROR_UNCORRECT_DATA;
    }
    data.swap( tmp );
    return true;
}

void VanHao::CVanHao::clearFromScob( std::forward_list<std::string>& data ) {
    for ( auto iter = data.begin(); iter != data.end(); ++iter ) {
        deleteScob( *iter );
    }
}

std::string VanHao::CVanHao::nextProcessing( const std::forward_list<std::string>& leftData,
                                             const std::forward_list<std::string>& rightData ) {
    std::tr1::cmatch res;
    std::forward_list<std::string> dataCopy = leftData;
    for ( auto iter = dataCopy.begin(); iter != dataCopy.end(); ++iter ) {
        if ( std::tr1::regex_search( iter->c_str(), res, regexOr_ ) ) {
            std::string leftOpData = res [ 1 ].str(),
                rightOpData = res [ 2 ].str();
            std::string dataOpCopy = *iter;
            try {
                *iter = leftOpData;
                return processing( dataCopy, rightData );
            } catch ( const char * er ) {
                if ( !uncorrectData.empty() )
                    throw er;
            }
            try {
                *iter = rightOpData;
                return processing( dataCopy, rightData );
            } catch ( const char * er ) {
                if ( !uncorrectData.empty() )
                    throw er;
            }
            *iter = dataOpCopy;
        }
    }
    dataCopy = rightData;
    for ( auto iter = dataCopy.begin(); iter != dataCopy.end(); ++iter ) {
        if ( std::tr1::regex_search( iter->c_str(), res, regexAnd_ ) ) {
            if ( res [ 1 ].second [ 0 ] != '&' )
                continue;
            std::string leftOpData = res [ 1 ].str(),
                rightOpData = res [ 2 ].str();
            std::string dataOpCopy = *iter;
            try {
                *iter = leftOpData;
                return processing( leftData, dataCopy );
            } catch ( const char * er ) {
                if ( !uncorrectData.empty() )
                    throw er;
            }
            try {
                *iter = rightOpData;
                return processing( leftData, dataCopy );
            } catch ( const char * er ) {
                if ( !uncorrectData.empty() )
                    throw er;
            }
            *iter = dataOpCopy;
        }
    }
    throw VAN_HAO_ERROR_HAVENOT_ANSWER;
}

bool VanHao::CVanHao::isSimpleData( const std::forward_list<std::string>& data ) {
    for ( auto i = data.begin(); i != data.end(); ++i ) {
        if ( !std::tr1::regex_search( *i, regexSimpleData_ ) ) {
            return false;
        }
    }
    return true;
}

bool VanHao::CVanHao::deleteImplication( std::forward_list<std::string>& data ) {
    std::tr1::cmatch res;
    bool isDelete = false,
        isMake = false;
    
    do {
        isDelete = false;
        std::forward_list<std::string> tmp;
        for ( auto iter = data.begin(); iter != data.end(); ++iter ) {
            if ( std::tr1::regex_search( iter->c_str(), res, regexImpl_ ) ) {
                std::string leftData = res [ 1 ].str(),
                    rightData = res [ 2 ].str();
                std::string d = (*iter);
                d.resize( ( d.size() - leftData.size() - strlen( res [ 1 ].second ) ) );
                
                std::tr1::cmatch resLeftData;
                if ( std::tr1::regex_search( leftData.c_str(), resLeftData, regexNot_ ) ) {
                    leftData = resLeftData [ 1 ].str();
                    d += '(';
                }
                else
                    d += "~(";
                deleteScob(leftData);
                d += leftData;
                d += ")|";
                deleteScob(rightData);
                if ( std::tr1::regex_search( rightData, regexNot_ ) ||
                     std::tr1::regex_search( rightData, regexSimpleData_ ) )
                    d += rightData;
                else
                    d += '(' + rightData + ')';
                d += res [ 2 ].second;
                tmp.push_front( d );
                isDelete = true;
                isMake = true;
                continue;
            }
            tmp.push_front( *iter );
        }
        data.swap( tmp );
    } while ( isDelete );
    return isMake;
}

bool VanHao::CVanHao::deleteOperation( std::forward_list<std::string> & data, 
                                       std::tr1::regex regex ) {
    std::tr1::cmatch res;
    bool isDelete = false,
        isMake = false;
    do {
        isDelete = false;
        std::forward_list<std::string> tmp;
        for ( auto iter = data.begin(); iter != data.end(); ++iter ) {
            if ( std::tr1::regex_search( iter->c_str(), res, regex ) ) {
                std::string leftData = res [ 1 ].str(),
                    rightData = res [ 2 ].str();
                    deleteScob( leftData );
                    tmp.push_front( leftData );
                    deleteScob( rightData );
                    tmp.push_front( rightData );
                    isDelete = true;
                    isMake = true;
                    continue;

            }
            tmp.push_front( *iter );
        }
        tmp = deleteRepeatElem( tmp );
        data.swap( tmp );
        
    } while ( isDelete );
    return isMake;
}

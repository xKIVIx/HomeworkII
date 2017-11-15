#pragma once
#include <string>
#include <forward_list>
#include <regex>
namespace VanHao {
    class CVanHao {
    public:
        std::string handle( const std::string & leftData,
                            const std::string & rightData );

        CVanHao();
    private:
        std::tr1::regex regexSimpleData_;
        std::tr1::regex regexIsScob_;
        std::tr1::regex regexOr_;
        std::tr1::regex regexAnd_;
        std::tr1::regex regexImpl_;
        std::tr1::regex regexNot_;
        std::string uncorrectData;

        bool checkCorrect( const std::forward_list <std::string> & data );

        std::string packData( const std::forward_list <std::string> & data1,
                              const std::forward_list <std::string> & data2 );


        std::forward_list <std::string> getDataList( const std::string & data );


        void viewDataVector( const char * name,
                             const std::forward_list <std::string> & data );


        std::forward_list <std::string> deleteRepeatElem( const std::forward_list <std::string> & data );


        bool isEqualData( const std::forward_list <std::string> & data1,
                          const std::forward_list <std::string> & data2 );


        std::string processing( std::forward_list <std::string> leftData,
                                std::forward_list <std::string> rightData );


        bool departNot( std::forward_list <std::string> & leftData,
                        std::forward_list <std::string> & rightData );


        bool deleteScob( std::string & data );


        void clearFromScob( std::forward_list <std::string> & data );


        bool VanHao::CVanHao::deleteOperation( std::forward_list<std::string> & data,
                                               std::tr1::regex regex );

        std::string nextProcessing( const std::forward_list <std::string>& leftData,
                                    const std::forward_list <std::string>& rightData );

        bool isSimpleData( const std::forward_list <std::string>& data );

        bool deleteImplication( std::forward_list <std::string>& data );

    };
}


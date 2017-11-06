#pragma once
#include "CClasterisator.h"
namespace Claster {
    class CClasterisatorMask :CClasterisator {
    public:
        virtual std::string clasterisation( std::string data );
    private:
        unsigned int minDeff_ = 0;
        std::vector <unsigned int> masks_;
        std::vector <std::vector <bool>> identGroups_;
        std::vector <CObject *> clastersHeads_;
        
        void initMasks( std::string & data );
        void initIdentGroups();
        void sreachClasters();
        bool isEqualGroup( unsigned int firstGroup,
                           unsigned int secondGroup );
    };
}


#include "server/Transcoder.hpp"

#include <iostream>

namespace tr {
    Transcoder::Transcoder() {

    }


    Transcoder::~Transcoder() {

    }


    void Transcoder::printServerInfo() { 
        std::cout << "==========================================" << std::endl;
        std::cout << "= Transcoder Server Start                 " << std::endl;
        std::cout << "==========================================" << std::endl;
    }


    void Transcoder::start() { 
        this->printServerInfo();
    }
};

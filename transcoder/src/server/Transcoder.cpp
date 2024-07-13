#include "server/Transcoder.hpp"

#include <iostream>

#include "drogon/drogon.h"

namespace tr {
    Transcoder Transcoder::instance;

    Transcoder& Transcoder::getInstance() {
        return Transcoder::instance;
    }

    Transcoder::Transcoder() {}

    Transcoder::~Transcoder() {}

    void Transcoder::printServerInfo() {
        std::cout << "==========================================" << std::endl;
        std::cout << "= Transcoder Server Start                 " << std::endl;
        std::cout << "= Port: " << Config::getPort() << std::endl;
        std::cout << "==========================================" << std::endl;
    }

    void Transcoder::start() {
        this->printServerInfo();

        drogon::app()
            .setUploadPath("./upload")
            .setClientMaxBodySize(10 * 1024 * 1024 * 1024LL)  // 10GB ���� ���ε� ����
            .setClientMaxMemoryBodySize(1 * 1024 * 1024)      // 1MB �޸� ��� ����
            .addListener("0.0.0.0", Config::getPort())
            .setThreadNum(Config::getThreadNum())
            .run();
    }
}  // namespace tr

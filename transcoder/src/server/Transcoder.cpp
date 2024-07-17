#include "server/Transcoder.hpp"

#include <iostream>

#include "drogon/drogon.h"
#include "http/HTTPFileHandler.hpp"
#include "http/HTTPWebSocketHandler.hpp"
#include "http/HTTPSessionFilter.hpp"

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

        drogon::app().registerPreRoutingAdvice([](const drogon::HttpRequestPtr& req, drogon::AdviceCallback&& callback,
                                                  drogon::AdviceChainCallback&& chain) {
            if (req->method() == drogon::HttpMethod::Options) {
                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->addHeader("Access-Control-Allow-Origin", "http://127.0.0.1:8080");
                resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
                resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
                callback(resp);
                return;
            }
            chain();
        });

        drogon::app()
            .setUploadPath("./upload")
            .setClientMaxBodySize(10 * 1024 * 1024 * 1024LL)  // 10GB ���� ���ε� ����
            .setClientMaxMemoryBodySize(1 * 1024 * 1024)      // 1MB �޸� ��� ����
            .addListener("0.0.0.0", Config::getPort())
            .setThreadNum(Config::getThreadNum())
            .run();
    }

    void Transcoder::stop() {
        drogon::app().quit();
    }

}  // namespace tr

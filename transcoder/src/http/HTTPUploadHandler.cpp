#include "http/HTTPUploadHandler.hpp"

namespace tr {
    HTTPUploadHandler::HTTPUploadHandler() {}

    HTTPUploadHandler::~HTTPUploadHandler() {}

    void HTTPUploadHandler::fileUpload(const drogon::HttpRequestPtr& req,
                                       std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k200OK);
        resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
        resp->setBody("Hello, World!");
        callback(resp);
    }

    void HTTPUploadHandler::test() {
        std::cout << "tets" << std::endl;
    }
};

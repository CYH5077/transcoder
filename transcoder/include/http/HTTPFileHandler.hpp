#pragma once

#include "drogon/drogon.h"

namespace tr {
    class HTTPFileHandler : public drogon::HttpController<HTTPFileHandler> {
    public:
        METHOD_LIST_BEGIN
        ADD_METHOD_TO(HTTPFileHandler::upload, "/file", drogon::Post);
        ADD_METHOD_TO(HTTPFileHandler::fileList, "/file/list", drogon::Get);
        ADD_METHOD_TO(HTTPFileHandler::download, "/file", drogon::Get);
        ADD_METHOD_TO(HTTPFileHandler::option, "/file", drogon::Options);
        METHOD_LIST_END

    public:
        void upload(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
        void fileList(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback);
        void download(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback);
        void option(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    };
};  // namespace tr

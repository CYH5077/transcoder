#pragma once

#include "drogon/drogon.h"
#include "http/HTTPSessionFilter.hpp"

namespace tr {
    class HTTPFileHandler : public drogon::HttpController<HTTPFileHandler> {
    public:
        METHOD_LIST_BEGIN
        ADD_METHOD_TO(HTTPFileHandler::upload, "/file", drogon::Post, "tr::HTTPSessionFilter");
        ADD_METHOD_TO(HTTPFileHandler::download, "/file", drogon::Get, "tr::HTTPSessionFilter");
        ADD_METHOD_TO(HTTPFileHandler::option, "/file", drogon::Options, "tr::HTTPSessionFilter");
        ADD_METHOD_TO(HTTPFileHandler::transcodeDownload, "/transcode", drogon::Get, "tr::HTTPSessionFilter");
        METHOD_LIST_END

    public:
        void upload(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
        void download(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback);
        void transcodeDownload(const drogon::HttpRequestPtr& req,
                               std::function<void(const drogon::HttpResponsePtr&)>&& callback);
        void option(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    private:
        void downloadFile(const std::string& path,
                          const drogon::HttpRequestPtr& req,
                          std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    };
};  // namespace tr

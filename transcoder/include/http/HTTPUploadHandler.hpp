#pragma once

#include "drogon/drogon.h"

namespace tr {
    class HTTPUploadHandler : public drogon::HttpController<HTTPUploadHandler> {
    public:
        METHOD_LIST_BEGIN
        ADD_METHOD_TO(HTTPUploadHandler::upload, "/upload", drogon::Post);
        METHOD_LIST_END

    public:
        explicit HTTPUploadHandler();
        virtual ~HTTPUploadHandler();

    public:
        void upload(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    };
};  // namespace tr

#pragma once

#include "drogon/drogon.h"

namespace tr {
    class HTTPUploadHandler : public drogon::HttpController<HTTPUploadHandler> {
    public:
        explicit HTTPUploadHandler();
        virtual ~HTTPUploadHandler();

    public:
        METHOD_LIST_BEGIN

        METHOD_LIST_END

    public:
        void fileUpload(const drogon::HttpRequestPtr& req,
                        std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    };
};  // namespace tr

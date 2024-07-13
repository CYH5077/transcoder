#include "http/HTTPUploadHandler.hpp"

namespace tr {
    HTTPUploadHandler::HTTPUploadHandler() {}

    HTTPUploadHandler::~HTTPUploadHandler() {}

    void HTTPUploadHandler::upload(const drogon::HttpRequestPtr& req,
                                   std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        int ret = 0;
        drogon::MultiPartParser multiPartParser;
        auto response = drogon::HttpResponse::newHttpResponse();

        ret = multiPartParser.parse(req);
        if (ret != 0) {
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            response->setBody("Bad Request");
            callback(response);
            return;
        }

        LOG_INFO << "Upload File Count: " << multiPartParser.getFiles().size();

        auto files = multiPartParser.getFiles();
        for (const auto& file : files) {
            LOG_INFO << "File Name: " << file.getFileName();
			LOG_INFO << "File Size: " << file.fileLength();
			LOG_INFO << "File Content Type: " << file.getContentType();

            int ret = file.save();
            if (ret != 0) {
				response->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
				response->setBody("Internal Server Error");
				callback(response);
				return;
			}
        }

        response->setStatusCode(drogon::HttpStatusCode::k200OK);
        response->setBody("Upload Success");
        callback(response);
    }
};  // namespace tr

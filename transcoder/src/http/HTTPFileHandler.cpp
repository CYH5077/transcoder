#include "http/HTTPFileHandler.hpp"

namespace tr {
    void HTTPFileHandler::upload(const drogon::HttpRequestPtr& req,
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

    void HTTPFileHandler::download(const drogon::HttpRequestPtr& req,
                                   std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        // URL에서 파일 이름 파라미터 추출
        // req body에서 json을 파싱한다.파싱한다
        auto requestJson = req->getJsonObject();

        Json::Value errorJson;
        errorJson["type"] = "error";

        std::string fileName;
        try {
            fileName = (*requestJson)["file"].asString();
        } catch (const Json::Exception& e) {
            errorJson["error_message"] = "Invalid JSON";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(errorJson);
            resp->setStatusCode(drogon::k400BadRequest);
            callback(resp);
            return;
        }

        // 파일 경로 설정 
        std::string filePath = drogon::app().getUploadPath() + "/" + fileName;

        // 파일 존재 여부 확인
        if (!std::filesystem::exists(filePath)) {
            errorJson["error_message"] = "File not found";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(errorJson);
            resp->setStatusCode(drogon::k404NotFound);
            callback(resp);
            return;
        }

        // 파일 응답 생성
        auto resp = drogon::HttpResponse::newFileResponse(filePath);
        callback(resp);
    }

    void HTTPFileHandler::option(const drogon::HttpRequestPtr& req,
                                 std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k204NoContent);
        resp->addHeader("Access-Control-Allow-Origin", "http://125.177.14.20:10000");
        resp->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
        callback(resp);
    }
};  // namespace tr

#include "http/HTTPFileHandler.hpp"

namespace tr {
    void HTTPFileHandler::upload(const drogon::HttpRequestPtr& req,
                                 std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        int ret = 0;
        drogon::MultiPartParser multiPartParser;

        ret = multiPartParser.parse(req);
        if (ret != 0) {
            auto response = drogon::HttpResponse::newHttpResponse();
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
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
                response->setBody("Internal Server Error");
                callback(response);
                return;
            }
        }

        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::HttpStatusCode::k200OK);
        response->setBody("Upload Success");
        callback(response);
    }

    void HTTPFileHandler::fileList(const drogon::HttpRequestPtr& req,
                                   std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        std::string path = drogon::app().getUploadPath();
        std::vector<std::string> fileList;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (std::filesystem::is_regular_file(entry.status())) {
                    fileList.push_back(entry.path().filename().string());
                }
            }

            Json::Value response;
            response["files"] = Json::Value(Json::arrayValue);
            for (const auto& file : fileList) {
                response["files"].append(file);
            }

            auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
            callback(resp);
        } catch (const std::filesystem::filesystem_error& e) {
            Json::Value errorMessage;
            errorMessage["error"] = e.what();

            auto resp = drogon::HttpResponse::newHttpJsonResponse(errorMessage);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }
    }

    void HTTPFileHandler::download(const drogon::HttpRequestPtr& req,
                                   std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        // URL에서 파일 이름 파라미터 추출
        // req body에서 json을 파싱한다.파싱한다
        auto json = req->getJsonObject();

        std::string fileName;
        try {
            fileName = (*json)["file"].asString();
        } catch (const Json::Exception& e) {
            Json::Value json;
            json["error"] = "Invalid JSON";
            // JSON 파싱 오류 처리
            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::k400BadRequest);
            callback(resp);
            return;
        }

        if (fileName.empty()) {
            Json::Value json;
            json["error"] = "File name not set";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::k400BadRequest);
            callback(resp);
            return;
        }

        // 파일 경로 설정 (실제 환경에 맞게 조정 필요)
        std::string filePath = drogon::app().getUploadPath() + "/" + fileName;

        // 파일 존재 여부 확인
        if (!std::filesystem::exists(filePath)) {
            Json::Value json;
            json["error"] = "File not found";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
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

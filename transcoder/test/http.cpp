#include <fstream>
#include <iostream>

#include "drogon/drogon.h"
#include "gtest/gtest.h"
#include "server/Transcoder.hpp"

TEST(HTTP_TEST, FILE_UPLOAD_DOWNLOAD) {
    std::string helloworldFile = "./helloworld.txt";

    // helloworld.txt Write File
    std::ofstream file(helloworldFile);
    ASSERT_TRUE(file.is_open());
    file << "Hello World!!" << std::endl;
    file.close();

    std::thread([] { drogon::app().run(); }).detach();
    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(8));
        std::cout << "app() stop" << std::endl;
        drogon::app().quit();
    }).detach();

    // 클라이언트 객체 생성
    auto client = drogon::HttpClient::newHttpClient("http://localhost:8080");

    // 요청 객체 생성
    drogon::UploadFile uploadFile(helloworldFile, "helloworld.txt");
    std::vector<drogon::UploadFile> files = {uploadFile};
    auto req = drogon::HttpRequest::newFileUploadRequest(files);

    // 비동기 요청 전송
    client->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok) {
            std::cout << "파일 업로드 성공. 응답 상태 코드: " << response->getStatusCode() << std::endl;
            std::cout << "서버 응답: " << response->getBody() << std::endl;
        } else {
            std::cout << "파일 업로드 실패" << std::endl;
        }
    }, 1);


    std::this_thread::sleep_for(std::chrono::seconds(10));

    // File Download
    /*
    drogon::HttpRequestPtr downloadRequest = drogon::HttpRequest::newHttpRequest("/download/helloworld.txt");
    auto downloadResponse = client->sendRequest(downloadRequest);
    ASSERT_EQ(downloadResponse->statusCode(), drogon::HttpStatusCode::k200OK);
    std::string downloadedFilePath = "/path/to/download/directory/helloworld.txt";
    downloadResponse->saveBody(downloadedFilePath);

    // File Compare
    std::ifstream uploadedFile(filePath);
    std::ifstream downloadedFile(downloadedFilePath);
    ASSERT_TRUE(uploadedFile.is_open());
    ASSERT_TRUE(downloadedFile.is_open());
    std::string uploadedContent((std::istreambuf_iterator<char>(uploadedFile)), std::istreambuf_iterator<char>());
    std::string downloadedContent((std::istreambuf_iterator<char>(downloadedFile)), std::istreambuf_iterator<char>());
    ASSERT_EQ(uploadedContent, downloadedContent);
    uploadedFile.close();
    downloadedFile.close();
    */
}

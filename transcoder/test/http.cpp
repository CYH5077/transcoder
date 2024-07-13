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

    // Ŭ���̾�Ʈ ��ü ����
    auto client = drogon::HttpClient::newHttpClient("http://localhost:8080");

    // ��û ��ü ����
    drogon::UploadFile uploadFile(helloworldFile, "helloworld.txt");
    std::vector<drogon::UploadFile> files = {uploadFile};
    auto req = drogon::HttpRequest::newFileUploadRequest(files);

    // �񵿱� ��û ����
    client->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok) {
            std::cout << "���� ���ε� ����. ���� ���� �ڵ�: " << response->getStatusCode() << std::endl;
            std::cout << "���� ����: " << response->getBody() << std::endl;
        } else {
            std::cout << "���� ���ε� ����" << std::endl;
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

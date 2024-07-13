#include <fstream>
#include <iostream>

#include "boost/beast.hpp"
#include "drogon/WebSocketClient.h"
#include "drogon/drogon.h"
#include "gtest/gtest.h"
#include "server/Config.hpp"

#ifdef __linux__
#include <netinet/tcp.h>
#include <sys/socket.h>
#endif

static std::string helloworldFile = "./helloworld.txt";

void requestUploadFile(drogon::HttpClientPtr client);
void requestGetFileList(drogon::HttpClientPtr client);
void requestDownloadFile(drogon::HttpClientPtr client);

TEST(HTTP_TEST, FILE_UPLOAD_DOWNLOAD) {
    // helloworld.txt Write File
    std::ofstream file(helloworldFile);
    ASSERT_TRUE(file.is_open());
    file << "Hello World!!" << std::endl;
    file.close();

    // HTTP client
    auto client = drogon::HttpClient::newHttpClient("http://127.0.0.1:10000");
    client->setSockOptCallback([](int fd) {
#ifdef __linux__
        int optval = 10;
        ::setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &optval, static_cast<socklen_t>(sizeof optval));
        ::setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &optval, static_cast<socklen_t>(sizeof optval));
        ::setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &optval, static_cast<socklen_t>(sizeof optval));
#endif
    });

    requestUploadFile(client);
    requestGetFileList(client);
    requestDownloadFile(client);
}

TEST(HTTP_TEST, WS_TEST) {
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::resolver resolver(ioContext);
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> webSocket(ioContext);

    // 127.0.0.1:10000 으로 connect 한다.
    boost::system::error_code error;
    auto const results = resolver.resolve("127.0.0.1", "10000");

    boost::asio::connect(webSocket.next_layer(), results, error);
    ASSERT_FALSE(error);
    webSocket.handshake("127.0.0.1", "/ws", error);
    ASSERT_FALSE(error);
    webSocket.write(boost::asio::buffer("Hello"), error);
    ASSERT_FALSE(error);
    std::cout << "Send Message" << std::endl;

    boost::beast::flat_buffer buffer;
    webSocket.read(buffer, error);
    ASSERT_FALSE(error);
    std::cout << "Recv Message: " << boost::beast::make_printable(buffer.data()) << std::endl;

    webSocket.close(boost::beast::websocket::close_code::normal, error);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void requestUploadFile(drogon::HttpClientPtr client) {
    // helloworld.txt UPLOAD
    drogon::UploadFile fileUpload(helloworldFile);
    auto req = drogon::HttpRequest::newFileUploadRequest({fileUpload});
    req->setMethod(drogon::HttpMethod::Post);
    req->setPath("/file");
    client->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok) {
            std::cout << "file upload success status code:" << response->getStatusCode() << std::endl;
        } else {
            std::cout << "file upload failed" << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::string uploadFilePath = drogon::app().getUploadPath() + "/" + helloworldFile;
    std::cout << uploadFilePath << std::endl;
    std::ifstream uploadedFile(uploadFilePath);
    ASSERT_TRUE(uploadedFile.is_open());
}

void requestGetFileList(drogon::HttpClientPtr client) {
    // GET 요청 생성
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setMethod(drogon::Get);
    req->setPath("/file/list");
    client->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result != drogon::ReqResult::Ok) {
            std::cerr << "Request failed: " << static_cast<int>(result) << std::endl;
            drogon::app().quit();
            return;
        }

        if (response->getStatusCode() == drogon::k200OK) {
            auto json = response->getJsonObject();
            if (json && (*json)["files"].isArray()) {
                std::cout << "file list:" << std::endl;
                for (const auto& file : (*json)["files"]) {
                    std::cout << "- " << file.asString() << std::endl;
                }
            } else {
                std::cerr << "Invalid json data" << std::endl;
            }
        } else {
            std::cerr << "Server Error StatusCode: " << response->getStatusCode() << std::endl;
            std::cerr << "Server Error ErrorMessage: " << response->body() << std::endl;
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void requestDownloadFile(drogon::HttpClientPtr client) {
    // helloworld.txt Download
    Json::Value json;
    json["file"] = helloworldFile;
    auto req = drogon::HttpRequest::newHttpJsonRequest(json);
    req->setPath("/file");
    req->setMethod(drogon::Get);
    client->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok) {
            if (response->getStatusCode() != drogon::k200OK) {
                std::cerr << "Download failed Status code: " << response->getStatusCode() << std::endl;
                std::cerr << "Download failed Error Message: " << response->body() << std::endl;
                return;
            }
            auto body = response->getBody();
            std::ofstream file("downloaded_file.txt", std::ios::binary);
            file.write(body.data(), body.size());
            file.close();
            std::cout << "file download success!" << std::endl;
        } else {
            std::cout << "file download failed" << std::endl;
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // downloaded_file.txt exist check
    std::ifstream downloadedFile("downloaded_file.txt");
    ASSERT_TRUE(downloadedFile.is_open());
}
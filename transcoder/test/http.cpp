#include <fstream>
#include <iostream>

#include "drogon/WebSocketClient.h"
#include "drogon/drogon.h"
#include "gtest/gtest.h"
#include "server/Config.hpp"

#ifdef __linux__
#include <netinet/tcp.h>
#include <sys/socket.h>
#endif

#include "http_test.hpp"

TEST(HTTP_TEST, FILE_UPLOAD_DOWNLOAD) {
    // helloworld.txt Write File
    std::ofstream file(HELLOWORLD_FILE);
    ASSERT_TRUE(file.is_open());
    file << "Hello World!!" << std::endl;
    file.close();

    // HTTP client
    auto client = drogon::HttpClient::newHttpClient(TRANSCODER_HTTP_URL);
#ifdef __linux__
    client->setSockOptCallback([](int fd) {
        int optval = 10;
        ::setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &optval, static_cast<socklen_t>(sizeof optval));
        ::setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &optval, static_cast<socklen_t>(sizeof optval));
        ::setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &optval, static_cast<socklen_t>(sizeof optval));
    });
#endif

    requestUploadDownload(client);
}

TEST(HTTP_TEST, WS_FILE_LIST) {
    wsRequestFileList();
}

TEST(HTTP_TEST, WS_REQUEST_TYPE_ERROR_INT) {
    wsRequestIntTypeError();
}

TEST(HTTP_TEST, WS_REQUEST_INVALID_TYPE_ERROR_STR) {
    wsRequestInvalidType();
}

TEST(HTTP_TEST, WS_REQUEST_INVLAID_TRANSCODE_FILE) {
    wsRequestTranscodeError();
}

TEST(HTTP_TEST, WS_REQUEST_TRANSCODE) {
    wsRequestTranscode();
}

TEST(HTTP_TEST, WS_REQUEST_TRANSCODED_FILE_LIST_AND_DOWNLOAD) {
    wsRequestTranscodedFileListAndDownload();
}

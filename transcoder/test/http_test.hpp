#pragma once

#define TRANSCODER_WEBSOCKET_URL std::string("ws://127.0.0.1:10000")
#define TRANSCODER_HTTP_URL std::string("http://127.0.0.1:10000")
#define HELLOWORLD_FILE std::string("helloworld.txt")
#define SAMPLE_MP4 std::string("sample.mp4")

void requestUploadFile(drogon::HttpClientPtr client);
void requestDownloadFile(drogon::HttpClientPtr client);

void jsonParse(const std::string& jsonStr, Json::Value* json);
void wsRequestFileList();
void wsRequestIntTypeError();
void wsRequestInvalidType();
void wsRequestTranscodeError();
void wsRequestTranscode();

// File upload test
void requestUploadFile(drogon::HttpClientPtr client) {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();

    // helloworld.txt UPLOAD
    drogon::UploadFile fileUpload(HELLOWORLD_FILE);
    auto req = drogon::HttpRequest::newFileUploadRequest({fileUpload});
    req->setMethod(drogon::HttpMethod::Post);
    req->setPath("/file");
    client->sendRequest(req, [&](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok) {
            std::cout << "file upload success status code:" << response->getStatusCode() << std::endl;
            if (response->getStatusCode() == drogon::k200OK) {
                promise.set_value(true);
            } else {
                promise.set_value(false);
            }
        } else {
            std::cout << "file upload failed" << std::endl;
            promise.set_value(false);
        }
    });

    while (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ASSERT_TRUE(future.get());
}

// File download test
void requestDownloadFile(drogon::HttpClientPtr client) {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();

    // helloworld.txt Download
    Json::Value json;
    json["file"] = HELLOWORLD_FILE;

    auto req = drogon::HttpRequest::newHttpJsonRequest(json);
    req->setPath("/file");
    req->setMethod(drogon::Get);

    client->sendRequest(req, [&](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
        if (result == drogon::ReqResult::Ok) {
            if (response->getStatusCode() != drogon::k200OK) {
                std::cerr << "Download failed Status code: " << response->getStatusCode() << std::endl;
                std::cerr << "Download failed Error Message: " << response->body() << std::endl;
                promise.set_value(false);
            } else {
                auto body = response->getBody();
                std::ofstream file("downloaded_file.txt", std::ios::binary);
                file.write(body.data(), body.size());
                file.close();
                std::cout << "file download success!" << std::endl;
                promise.set_value(true);
            }
        } else {
            std::cout << "file download failed" << std::endl;
            promise.set_value(false);
        }
    });

    while (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ASSERT_TRUE(future.get());
}

// get file list test
void wsRequestFileList() {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();

    // Websocket client
    auto wsClient = drogon::WebSocketClient::newWebSocketClient(TRANSCODER_WEBSOCKET_URL);

    // Recv message
    wsClient->setMessageHandler([&](const std::string& message,
                                    const drogon::WebSocketClientPtr& wsPtr,
                                    const drogon::WebSocketMessageType& type) {
        std::cout << "Received message: " << message << std::endl;
        Json::Value json;
        jsonParse(message, &json);
        if (json["type"].asString() == "file_list" && json["file_list"][0] == HELLOWORLD_FILE) {
            promise.set_value(true);
        } else {
            promise.set_value(false);
        }
    });

    // HTTP Request
    auto request = drogon::HttpRequest::newHttpRequest();
    request->setPath("/ws");

    // Connect
    wsClient->connectToServer(request,
                              [&](drogon::ReqResult result,
                                  const drogon::HttpResponsePtr& response,
                                  const drogon::WebSocketClientPtr& wsPtr) {
                                  if (result == drogon::ReqResult::Ok) {
                                      std::cout << "Connected to server" << std::endl;
                                      Json::Value json;
                                      json["task"] = "file_list";
                                      wsPtr->getConnection()->send(json.toStyledString());
                                  } else {
                                      promise.set_value(false);
                                      std::cerr << "Failed to connect to server" << std::endl;
                                  }
                              });

    while (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ASSERT_TRUE(future.get());
}

void jsonParse(const std::string& jsonStr, Json::Value* json) {
    Json::CharReaderBuilder jsonBuilder;
    std::string errors;
    std::istringstream iss(jsonStr);

    ASSERT_TRUE(Json::parseFromStream(jsonBuilder, iss, json, &errors));
}

void wsRequestIntTypeError() {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();

    // Websocket client
    auto wsClient = drogon::WebSocketClient::newWebSocketClient(TRANSCODER_WEBSOCKET_URL);

    // Recv message
    wsClient->setMessageHandler([&](const std::string& message,
                                    const drogon::WebSocketClientPtr& wsPtr,
                                    const drogon::WebSocketMessageType& type) {
        std::cout << "Received message: " << message << std::endl;
        Json::Value json;
        jsonParse(message, &json);
        if (json["type"].asString() == "error") {
            promise.set_value(true);
        } else {
            promise.set_value(false);
        }
    });

    // HTTP Request
    auto request = drogon::HttpRequest::newHttpRequest();
    request->setPath("/ws");

    // Connect
    wsClient->connectToServer(request,
                              [&](drogon::ReqResult result,
                                  const drogon::HttpResponsePtr& response,
                                  const drogon::WebSocketClientPtr& wsPtr) {
                                  if (result == drogon::ReqResult::Ok) {
                                      std::cout << "Connected to server" << std::endl;
                                      Json::Value json;
                                      // =============== Invalid type ===============
                                      json["task"] = 100;
                                      wsPtr->getConnection()->send(json.toStyledString());
                                  } else {
                                      promise.set_value(false);
                                      std::cerr << "Failed to connect to server" << std::endl;
                                  }
                              });

    while (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ASSERT_TRUE(future.get());
}

void wsRequestInvalidType() {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();

    // Websocket client
    auto wsClient = drogon::WebSocketClient::newWebSocketClient(TRANSCODER_WEBSOCKET_URL);

    // Recv message
    wsClient->setMessageHandler([&](const std::string& message,
                                    const drogon::WebSocketClientPtr& wsPtr,
                                    const drogon::WebSocketMessageType& type) {
        std::cout << "Received message: " << message << std::endl;
        Json::Value json;
        jsonParse(message, &json);
        if (json["type"].asString() == "error") {
            promise.set_value(true);
        } else {
            promise.set_value(false);
        }
    });

    // HTTP Request
    auto request = drogon::HttpRequest::newHttpRequest();
    request->setPath("/ws");

    // Connect
    wsClient->connectToServer(request,
                              [&](drogon::ReqResult result,
                                  const drogon::HttpResponsePtr& response,
                                  const drogon::WebSocketClientPtr& wsPtr) {
                                  if (result == drogon::ReqResult::Ok) {
                                      std::cout << "Connected to server" << std::endl;
                                      Json::Value json;
                                      // =============== Invalid type ===============
                                      json["task"] = "this is error!";
                                      wsPtr->getConnection()->send(json.toStyledString());
                                  } else {
                                      promise.set_value(false);
                                      std::cerr << "Failed to connect to server" << std::endl;
                                  }
                              });

    while (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ASSERT_TRUE(future.get());
}

void wsRequestTranscodeError() {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();

    // Websocket client
    auto wsClient = drogon::WebSocketClient::newWebSocketClient(TRANSCODER_WEBSOCKET_URL);

    // Recv message
    wsClient->setMessageHandler([&](const std::string& message,
                                    const drogon::WebSocketClientPtr& wsPtr,
                                    const drogon::WebSocketMessageType& type) {
        std::cout << "Received message: " << message << std::endl;
        Json::Value json;
        jsonParse(message, &json);
        if (json["type"].asString() == "error") {
            promise.set_value(true);
        } else {
            promise.set_value(false);
        }
    });

    // HTTP Request
    auto request = drogon::HttpRequest::newHttpRequest();
    request->setPath("/ws");

    // Connect
    wsClient->connectToServer(request,
                              [&](drogon::ReqResult result,
                                  const drogon::HttpResponsePtr& response,
                                  const drogon::WebSocketClientPtr& wsPtr) {
                                  if (result == drogon::ReqResult::Ok) {
                                      std::cout << "Connected to server" << std::endl;
                                      Json::Value json;
                                      // =============== Invalid type ===============
                                      json["task"] = "transcode";
                                      json["file"] = HELLOWORLD_FILE;
                                      wsPtr->getConnection()->send(json.toStyledString());
                                  } else {
                                      promise.set_value(false);
                                      std::cerr << "Failed to connect to server" << std::endl;
                                  }
                              });

    while (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ASSERT_TRUE(future.get());
}

void wsRequestTranscode() {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();

    // Websocket client
    auto wsClient = drogon::WebSocketClient::newWebSocketClient(TRANSCODER_WEBSOCKET_URL);

    // Recv message
    wsClient->setMessageHandler([&](const std::string& message,
                                    const drogon::WebSocketClientPtr& wsPtr,
                                    const drogon::WebSocketMessageType& type) {
        std::cout << "Received message: " << message << std::endl;
        Json::Value json;
        jsonParse(message, &json);
        if (json["result"].asString() == "finish") {
            promise.set_value(true);
        } else if (json["type"].asString() == "error") {
            promise.set_value(false);
        }
    });

    // HTTP Request
    auto request = drogon::HttpRequest::newHttpRequest();
    request->setPath("/ws");

    // Connect
    wsClient->connectToServer(request,
                              [&](drogon::ReqResult result,
                                  const drogon::HttpResponsePtr& response,
                                  const drogon::WebSocketClientPtr& wsPtr) {
                                  if (result == drogon::ReqResult::Ok) {
                                      std::cout << "Connected to server" << std::endl;
                                      Json::Value json;
                                      // =============== Invalid type ===============
                                      json["task"] = "transcode";
                                      json["file"] = SAMPLE_MP4;
                                      wsPtr->getConnection()->send(json.toStyledString());
                                  } else {
                                      promise.set_value(false);
                                      std::cerr << "Failed to connect to server" << std::endl;
                                  }
                              });

    while (future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ASSERT_TRUE(future.get());
}

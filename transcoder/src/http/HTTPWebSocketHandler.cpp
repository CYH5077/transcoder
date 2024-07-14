#include "http/HTTPWebSocketHandler.hpp"

namespace tr {
    void HTTPWebSocketHandler::handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                                                std::string&& message,
                                                const drogon::WebSocketMessageType& type) {
        if (type == drogon::WebSocketMessageType::Text) {
            this->thread = std::make_shared<std::thread>([this, conn]() {
                this->isRunning = true;
                while (this->isRunning && !conn->disconnected()) {
                    Json::Value json;
                    json["hello"] = "Hello World";
                    conn->sendJson(json);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                LOG_INFO << "Send message thread exit";
            });
        }
    }

    void HTTPWebSocketHandler::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) {
        LOG_INFO << "Close  connection client IP: " << conn->peerAddr().toIpPort();
        this->isRunning = false;
        this->thread->join();
    }

    void HTTPWebSocketHandler::handleNewConnection(const drogon::HttpRequestPtr& requset,
                                                   const drogon::WebSocketConnectionPtr& conn) {
        // 접속한 클라이언트의 IP 를 출력한다.
        LOG_INFO << "New connection client IP: " << conn->peerAddr().toIpPort();
    }
}

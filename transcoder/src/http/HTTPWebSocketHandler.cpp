#include "http/HTTPWebSocketHandler.hpp"

#include <mutex>

#include "dto/DtoWSErrorResponse.hpp"

namespace tr {
    void HTTPWebSocketHandler::handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                                                std::string&& message,
                                                const drogon::WebSocketMessageType& type) {
        if (type == drogon::WebSocketMessageType::Text) {
            WSClientPtr client;

            // get WSClient
            {
                std::lock_guard<std::mutex> lock(this->clientsMutex);
                if (clients.find(conn) != clients.end()) {
                    client = clients[conn];
                }
            }

            // 왜 map 에 없을까
            if (client == nullptr) {
                LOG_INFO << "Client not found";
                conn->forceClose();
                return;
            }

            // json parse
            std::shared_ptr<Json::Value> json = std::make_shared<Json::Value>();
            Json::CharReaderBuilder jsonBuilder;
            std::string errors;
            std::istringstream iss(message);

            if (Json::parseFromStream(jsonBuilder, iss, json.get(), &errors)) {
                this->taskManager.run(client, json);
            } else {
                client->sendResponse(DtoWSErrorResponse::createErrorMessage("Invalid JSON format: " + errors));
            }
        }
    }

    void HTTPWebSocketHandler::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) {
        LOG_INFO << "Close  connection client IP: " << conn->peerAddr().toIpPort();

        {
            std::lock_guard<std::mutex> lock(this->clientsMutex);
            WSClientPtr client = clients[conn];
            client->setConnect(false);
            clients.erase(conn);
        }
    }

    void HTTPWebSocketHandler::handleNewConnection(const drogon::HttpRequestPtr& requset,
                                                   const drogon::WebSocketConnectionPtr& conn) {
        // 접속한 클라이언트의 IP 를 출력한다.
        LOG_INFO << "New connection client IP: " << conn->peerAddr().toIpPort();

        {
            std::lock_guard<std::mutex> lock(this->clientsMutex);
            WSClientPtr client = WSClient::create(conn);
            client->setConnect(true);
            clients[conn] = client;
        }
    }
}

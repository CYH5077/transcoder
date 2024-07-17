#include "http/HTTPSessionManager.hpp"

#include "drogon/drogon.h"

namespace tr {
    void HTTPSessionManager::addClient(const drogon::WebSocketConnectionPtr& conn) {
        std::lock_guard<std::mutex> lock(this->clientsMutex);
        this->clients[conn] = WSClient::create(conn);
    }

    void HTTPSessionManager::removeClient(const drogon::WebSocketConnectionPtr& conn) {
        std::lock_guard<std::mutex> lock(this->clientsMutex);
        this->clients.erase(conn);
    }

    WSClientPtr HTTPSessionManager::getClient(const drogon::WebSocketConnectionPtr& conn) {
        if (this->hasClient(conn) == false) {
			return nullptr;
		}

        std::lock_guard<std::mutex> lock(this->clientsMutex);
        return this->clients[conn];
    }

    WSClientPtr HTTPSessionManager::getClient(const std::string& sessionId) {
        std::lock_guard<std::mutex> lock(this->clientsMutex);
        for (const auto& client : this->clients) {
            if (client.second == sessionId) {
                return client.second;
            }
        }

        return nullptr;
    }

    bool HTTPSessionManager::hasClient(const drogon::WebSocketConnectionPtr& conn) {
        std::lock_guard<std::mutex> lock(this->clientsMutex);
        return this->clients.find(conn) != this->clients.end();
    }
}
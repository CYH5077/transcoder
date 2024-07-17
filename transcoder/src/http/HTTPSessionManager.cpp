#include "http/HTTPSessionManager.hpp"

#include "drogon/drogon.h"

namespace tr {
    HTTPSessionManager HTTPSessionManager::instance;

    HTTPSessionManager& HTTPSessionManager::getInstance() {
		return HTTPSessionManager::instance;
	}

    std::string HTTPSessionManager::addClient(const drogon::WebSocketConnectionPtr& conn) {
        std::lock_guard<std::mutex> lock(this->clientsMutex);
        WSClientPtr client = WSClient::create(conn);
        client->setConnect(true);
        this->clients[conn] = client;
        return client->getSessionId();
    }

    void HTTPSessionManager::removeClient(const drogon::WebSocketConnectionPtr& conn) {
        WSClientPtr client = this->getClient(conn);
        if (client != nullptr) {
			client->setConnect(false);
		}

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

    WSClientPtr HTTPSessionManager::getClient(std::string& sessionId) {
        std::lock_guard<std::mutex> lock(this->clientsMutex);
        for (auto client : this->clients) {
            if (client.second->getSessionId() == sessionId) {
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

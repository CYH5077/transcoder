#pragma once

#include <map>
#include <mutex>
#include <string>

#include "drogon/drogon.h"
#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    class HTTPSessionManager {
    public:
        explicit HTTPSessionManager() = default;
        virtual ~HTTPSessionManager() = default;

    public:
        void addClient(const drogon::WebSocketConnectionPtr& conn, WSClientPtr client);
        void removeClient(const drogon::WebSocketConnectionPtr& conn);
        
        WSClientPtr getClient(const drogon::WebSocketConnectionPtr& conn);
        WSClientPtr getClient(const std::string& sessionId);

        bool hasClient(const drogon::WebSocketConnectionPtr& conn);

    private:
        std::mutex clientsMutex;
        std::map<const drogon::WebSocketConnectionPtr, WSClientPtr> clients;
    };
};

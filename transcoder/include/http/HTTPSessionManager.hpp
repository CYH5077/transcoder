#pragma once

#include <map>
#include <mutex>
#include <string>

#include "drogon/drogon.h"
#include "http/ws_recv_handler/WSClient.hpp"
#include "utils/ThreadSafeQueue.hpp"

namespace tr {
    class HTTPSessionManager {
    public:
        static HTTPSessionManager& getInstance();
        static HTTPSessionManager instance;

    public:
        std::string addClient(const drogon::WebSocketConnectionPtr& conn);
        void removeClient(const drogon::WebSocketConnectionPtr& conn);
        
        WSClientPtr getClient(const drogon::WebSocketConnectionPtr& conn);
        WSClientPtr getClient(std::string& sessionId);

        bool hasClient(const drogon::WebSocketConnectionPtr& conn);

    private:
        explicit HTTPSessionManager() = default;
        virtual ~HTTPSessionManager() = default;

    private:
        std::mutex clientsMutex;
        std::map<const drogon::WebSocketConnectionPtr, WSClientPtr> clients;
    };
};

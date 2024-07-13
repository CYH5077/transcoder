#pragma once

#include "HTTPWebSocketHandler.hpp"
#include "drogon/WebSocketController.h"
#include "drogon/drogon.h"

namespace tr {
    class HTTPWebSocketHandler : public drogon::WebSocketController<HTTPWebSocketHandler> {
    public:
        WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws", drogon::Get);
        WS_PATH_LIST_END

    public:
        void handleNewMessage(const drogon::WebSocketConnectionPtr&,
                              std::string&&,
                              const drogon::WebSocketMessageType&) override;
        void handleConnectionClosed(const drogon::WebSocketConnectionPtr&) override;
        void handleNewConnection(const drogon::HttpRequestPtr&, const drogon::WebSocketConnectionPtr&) override;

    private:
        bool isRunning = false;
        std::shared_ptr<std::thread> thread;
    };
};

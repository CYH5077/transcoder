#pragma once

#include "drogon/WebSocketController.h"
#include "drogon/drogon.h"

namespace tr {
    class HTTPWebSocketHandler : public drogon::WebSocketController<HTTPWebSocketHandler> {
    public:
        WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws", drogon::Get);
        WS_PATH_LIST_END

    public:
        void handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                              std::string&& message,
                              const drogon::WebSocketMessageType& type) override;
        void handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) override;
        void handleNewConnection(const drogon::HttpRequestPtr& request,
                                 const drogon::WebSocketConnectionPtr& conn) override;

    private:
        bool isRunning = false;
        std::shared_ptr<std::thread> thread;
    };
};

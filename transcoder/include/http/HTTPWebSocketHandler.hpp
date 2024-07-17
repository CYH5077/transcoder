#pragma once

#include <map>
#include <mutex>

#include "drogon/WebSocketController.h"
#include "drogon/drogon.h"
#include "http/ws_recv_handler/WSClient.hpp"
#include "http/ws_recv_handler/WSRequestManager.hpp"

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
        std::mutex clientsMutex;
        std::map<const drogon::WebSocketConnectionPtr, WSClientPtr> clients;

        WSRequestManager taskManager;
    };
};

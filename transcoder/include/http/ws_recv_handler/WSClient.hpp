#pragma once

#include <memory>

#include "drogon/WebSocketController.h"
#include "drogon/drogon.h"

namespace tr {
    class WSClient;
    using WSClientPtr = std::shared_ptr<WSClient>;

    class WSClient {
    public:
        static WSClientPtr create(drogon::WebSocketConnectionPtr conn);

    public:
        explicit WSClient(drogon::WebSocketConnectionPtr conn);
        virtual ~WSClient() = default;

    public:  // get set
        void setConnect(bool connected);
        bool isConnected();

    public:  // send to client
        void sendJson(std::shared_ptr<Json::Value> json);

    private:
        bool connected;

        drogon::WebSocketConnectionPtr conn;
    };
};

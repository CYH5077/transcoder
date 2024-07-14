#pragma once

#include <memory>

#include "drogon/WebSocketController.h"
#include "drogon/drogon.h"
#include "dto/DtoResponseJsonType.hpp"

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
        void sendResponse(DtoResponseJsonTypePtr dto);

    private:
        bool connected;

        drogon::WebSocketConnectionPtr conn;
    };
};

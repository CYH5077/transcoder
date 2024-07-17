#pragma once

#include <memory>

#include "drogon/WebSocketController.h"
#include "drogon/drogon.h"
#include "dto/DtoResponseJsonType.hpp"
#include "dto/DtoWSTranscodeRequest.hpp"
#include "ffmpegpp.hpp"
#include "transcoder/TRANSCODE_DEFINE.hpp"

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
        // is connected
        void setConnect(bool connected);
        bool isConnected();

        // transcode state
        void setTranscodeState(TRANSCODE_STATE state);
        TRANSCODE_STATE getTranscodeState();

        // session
        std::string getSessionId();

    public:  // send to client
        void sendResponse(DtoResponseJsonTypePtr dto);

    private:
        bool connected;

        drogon::WebSocketConnectionPtr conn;

        TRANSCODE_STATE state;

        std::string sessionId;
    };
};

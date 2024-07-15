#pragma once

#include <memory>

#include "drogon/WebSocketController.h"
#include "drogon/drogon.h"
#include "dto/DtoResponseJsonType.hpp"
#include "dto/DtoWSTranscodeRequest.hpp"
#include "ffmpegpp.hpp"
#include "thread/TranscodeThread.hpp"

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

    public:
        void setTranscodeThread(TranscodeThreadPtr transcodeThread);
        void transcodeStart(std::shared_ptr<ff::FFAVInputContext> inputContext, DtoWSTranscodeRequestPtr request);
        void stopAll();

    private:
        bool connected;

        drogon::WebSocketConnectionPtr conn;

        TranscodeThreadPtr transcodeThread;
    };
};

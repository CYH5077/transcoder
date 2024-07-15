#pragma once

#include <memory>
#include <thread>

#include "dto/DtoWSTranscodeRequest.hpp"
#include "ffmpegpp.hpp"

namespace tr {
    class WSClient;
    using WSClientPtr = std::shared_ptr<WSClient>;

    class TranscodeThread;
    using TranscodeThreadPtr = std::shared_ptr<TranscodeThread>;

    class TranscodeThread {
    public:
        static TranscodeThreadPtr create(WSClientPtr client);

    public:
        explicit TranscodeThread(WSClientPtr& client);
        virtual ~TranscodeThread() = default;

    public:
        bool isRunning();

        void start(std::shared_ptr<ff::FFAVInputContext> inputContext, DtoWSTranscodeRequestPtr request);
        void stop();

    private:
        void transcode(std::shared_ptr<ff::FFAVInputContext> inputContext,
                       ff::FFAVVideoEncodeParametersPtr videoEncodeParameter,
                       ff::FFAVAudioEncodeParametersPtr audioEncodeParameter);


    private:
        bool running = false;
        std::shared_ptr<std::thread> thread;

        WSClientPtr client;
    };
};

#pragma once

#include <memory>
#include <thread>

#include "dto/DtoWSTranscodeRequest.hpp"
#include "ffmpegpp.hpp"
#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    class Transcoder;
    using TranscoderPtr = std::shared_ptr<Transcoder>;

    class Transcoder {
    public:
        static TranscoderPtr create(WSClientPtr client);

    public:
        explicit Transcoder(WSClientPtr client);
        virtual ~Transcoder() = default;

    public:
        void start(std::shared_ptr<ff::FFAVInputContext> inputContext, DtoWSTranscodeRequestPtr request);

    private:
        void transcode(std::shared_ptr<ff::FFAVInputContext> inputContext,
                       ff::FFAVVideoEncodeParametersPtr videoEncodeParameter,
                       ff::FFAVAudioEncodeParametersPtr audioEncodeParameter);

    private:
        WSClientPtr client;
    };
};

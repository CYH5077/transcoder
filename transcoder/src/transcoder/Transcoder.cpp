#include "transcoder/Transcoder.hpp"

#include "dto/DtoWSErrorResponse.hpp"
#include "dto/DtoWSTranscodeProgress.hpp"
#include "ffmpegpp.hpp"
#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    TranscoderPtr Transcoder::create(WSClientPtr client) {
        return std::make_shared<Transcoder>(client);
    }

    Transcoder::Transcoder(WSClientPtr client) : client(client) {}

    void Transcoder::start(std::shared_ptr<ff::FFAVInputContext> inputContext, DtoWSTranscodeRequestPtr request) {
        ff::FFAVTranscoderParameter parameter(*inputContext);

        ff::AVError error = parameter.createDecodeContext();
        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            this->client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage()));
            return;
        }
        auto videoEncodeParameters = parameter.getVideoEncodeParameters();
        auto audioEncodeParameters = parameter.getAudioEncodeParameters();
        videoEncodeParameters->setEncodeThreadCount(16);

        error = parameter.createEncodeCodecContext(ff::VIDEO_CODEC::H264, ff::AUDIO_CODEC::AAC);
        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            this->client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage()));
            return;
        }

        this->transcode(inputContext, parameter, request->getOutputFile());
    }

    void Transcoder::transcode(std::shared_ptr<ff::FFAVInputContext> inputContext,
                               ff::FFAVTranscoderParameter& parameter,
                               const std::string& outputFile) {
        ff::AVError error;

        ff::FFAVTranscoder transcoder(parameter);

        int decodeCount = 0;
        int max = parameter.getInputContext().getFrameCount();

        auto startTime = std::chrono::system_clock::now();
        transcoder.setDecodeCallback([&](ff::FFAVFrame&) {
            decodeCount++;

            auto nowTime = std::chrono::system_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - startTime).count() >= 350) {
                startTime = nowTime;
                this->client->sendResponse(DtoWSTranscodeProgress::createProgressMessage(decodeCount, max));
            }
        });

        transcoder.setFinishCallback(
            [&]() { this->client->sendResponse(DtoWSTranscodeProgress::createProgressMessage(decodeCount, max)); });

        transcoder.setErrorCallback([&](ff::ERROR_TYPE type, ff::AVError& error) {
            std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
            client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage()));
        });

        transcoder.transcode(outputFile);
    }
}  // namespace tr

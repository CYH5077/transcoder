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

#define FFAV_ERROR_CHECK_RETURN(error)                                                          \
    if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {                                        \
        this->client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage())); \
        return;                                                                                 \
    }

#define FFAV_ERROR_CHECK_BREAK(error)                                                           \
    if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {                                        \
        this->client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage())); \
        break;                                                                                  \
    }

    void Transcoder::transcode(std::shared_ptr<ff::FFAVInputContext> inputContext,
                               ff::FFAVTranscoderParameter& parameter,
                               const std::string& outputFile) {
        ff::AVError error;

        ff::FFAVTranscoder transcoder(parameter);
        transcoder.setErrorCallback([&](ff::ERROR_TYPE type, ff::AVError& error) {
            std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
            client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage()));
        });

        transcoder.transcode(outputFile);
    }
}  // namespace tr

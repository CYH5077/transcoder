#include "transcoder/Transcoder.hpp"

#include <filesystem>
#include <thread>

#include "dto/DtoWSErrorResponse.hpp"
#include "dto/DtoWSTranscodeProgress.hpp"
#include "ffmpegpp.hpp"
#include "http/ws_recv_handler/WSClient.hpp"
#include "server/Config.hpp"

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
        this->videoRequestToVideoParameter(request, videoEncodeParameters);
        this->audioRequestToAudioParameter(request, audioEncodeParameters);
        videoEncodeParameters->setMaxBFrames(10);
        videoEncodeParameters->setEncodeThreadCount(std::thread::hardware_concurrency());

        ff::VIDEO_CODEC videoCodec = request->getVideoCodec();
        if (videoCodec == ff::VIDEO_CODEC::NONE) {
            videoCodec = ff::VIDEO_CODEC::H264;
        }

        ff::AUDIO_CODEC audioCodec = request->getAudioCodec();
        if (audioCodec == ff::AUDIO_CODEC::NONE) {
            audioCodec = ff::AUDIO_CODEC::AAC;
        }

        error = parameter.createEncodeCodecContext(videoCodec, audioCodec);
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

        // Decode Callback
        auto startTime = std::chrono::system_clock::now();
        transcoder.setDecodeCallback([&](ff::FFAVFrame&) {
            decodeCount++;

            auto nowTime = std::chrono::system_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - startTime).count() >= 350) {
                startTime = nowTime;
                this->client->sendResponse(DtoWSTranscodeProgress::createProgressMessage(decodeCount, max));
            }
        });

        // Finish Callback
        transcoder.setFinishCallback([&]() {
            this->client->sendResponse(DtoWSTranscodeProgress::createProgressMessage(decodeCount, max));
            client->setTranscodeState(TRANSCODE_STATE::NONE);
            client->sendResponse(DtoWSTranscodeProgress::createFinishMessage(outputFile));
        });

        // Error Callback
        transcoder.setErrorCallback([&](ff::ERROR_TYPE type, ff::AVError& error) {
            std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
            client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage()));
        });

        std::string transcodeDir = Config::getInstance().getTranscodeDir();
        std::filesystem::create_directory(transcodeDir);

        transcoder.transcode(transcodeDir + "/" + outputFile);
    }

    void Transcoder::videoRequestToVideoParameter(DtoWSTranscodeRequestPtr request,
                                                  ff::FFAVVideoEncodeParametersPtr parameter) {
        if (request->getVideoBitrate() > 0) {
            parameter->setBitrate(request->getVideoBitrate());
        }

        if (request->getVideoWidth() > 0) {
            parameter->setWidth(request->getVideoWidth());
        }

        if (request->getVideoHeight() > 0) {
            parameter->setHeight(request->getVideoHeight());
        }

        if (request->getVideoGopSize() > 0) {
            parameter->setGOPSize(request->getVideoGopSize());
        }
    }

    void Transcoder::audioRequestToAudioParameter(DtoWSTranscodeRequestPtr request,
                                                  ff::FFAVAudioEncodeParametersPtr parameter) {
        if (request->getAudioBitrate() > 0) {
            parameter->setBitrate(request->getAudioBitrate());
        }

        if (request->getAudioSampleRate() > 0) {
            parameter->setSampleRate(request->getAudioSampleRate());
        }
    }

}  // namespace tr

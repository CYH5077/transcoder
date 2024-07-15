#include "transcoder/Transcoder.hpp"

#include "dto/DtoWSErrorResponse.hpp"
#include "dto/DtoWSTranscodeProgress.hpp"
#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    TranscoderPtr Transcoder::create(WSClientPtr client) {
        return std::make_shared<Transcoder>(client);
    }

    Transcoder::Transcoder(WSClientPtr client) : client(client) {}

    void Transcoder::start(std::shared_ptr<ff::FFAVInputContext> inputContext, DtoWSTranscodeRequestPtr request) {
        ff::FFAVVideoEncodeParametersPtr videoEncodeParameter = ff::FFAVVideoEncodeParameters::create(*inputContext);
        ff::FFAVAudioEncodeParametersPtr audioEncodeParameter = ff::FFAVAudioEncodeParameters::create(*inputContext);

        this->transcode(inputContext, videoEncodeParameter, audioEncodeParameter);
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
                               ff::FFAVVideoEncodeParametersPtr videoEncodeParameter,
                               ff::FFAVAudioEncodeParametersPtr audioEncodeParameter) {
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        //
        //
        //  트랜스 코딩 임시 테스트 코드 ffmpegpp쪽을 수정해야겠다.
        //
        //
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////

        ff::AVError error;
        ff::FFAVCodecContextPtr videoDecodeContext = ff::video::decode::createCodecContext(*inputContext, &error);
        FFAV_ERROR_CHECK_RETURN(error);
        ff::FFAVCodecContextPtr audioDecodeContext = ff::audio::decode::createCodecContext(*inputContext, &error);
        FFAV_ERROR_CHECK_RETURN(error);

        ff::FFAVCodecContextPtr videoEncodeContext =
            ff::video::encode::createCodecContext(ff::VIDEO_CODEC::H264, videoEncodeParameter, &error);
        FFAV_ERROR_CHECK_RETURN(error);
        ff::FFAVCodecContextPtr audioEncodeContext =
            ff::audio::encode::createCodecContext(ff::AUDIO_CODEC::AAC, audioEncodeParameter, &error);
        FFAV_ERROR_CHECK_RETURN(error);

        ff::FFAVOutputContext outputContext;
        error = outputContext.open(drogon::app().getUploadPath() + "/" + "output.mp4");
        FFAV_ERROR_CHECK_RETURN(error);

        outputContext.createStream(ff::DATA_TYPE::VIDEO, videoEncodeContext);
        outputContext.createStream(ff::DATA_TYPE::AUDIO, audioEncodeContext);
        error = outputContext.writeHeader();
        FFAV_ERROR_CHECK_RETURN(error);

        ff::FFAVDecoder decoder(videoDecodeContext, audioDecodeContext);
        ff::FFAVEncoder encoder(videoEncodeContext, audioEncodeContext);

        auto nowEpochTime = std::chrono::system_clock::now();

        error = decoder.decode(*inputContext, [&](ff::DATA_TYPE type, ff::FFAVFrame& frame) {
            if (this->client->isConnected() == false) {
                return false;
            }

            error = encoder.encode(type, frame, [&](ff::FFAVPacket& packet) {
                if (packet.getType() == ff::DATA_TYPE::VIDEO) {
                    packet.rescaleTS(inputContext->getVideoStream(), outputContext.getVideoStream());
                    packet.setStreamIndex(inputContext->getVideoStreamIndex());
                } else if (packet.getType() == ff::DATA_TYPE::AUDIO) {
                    packet.rescaleTS(inputContext->getAudioStream(), outputContext.getAudioStream());
                    packet.setStreamIndex(inputContext->getAudioStreamIndex());
                }

                // nowEpochTime 0.5 초 간격으로 프로그레스 정보를 전송한다.
                auto currentEpochTime = std::chrono::system_clock::now();
                long long milliseconds =
                    std::chrono::duration_cast<std::chrono::milliseconds>(currentEpochTime - nowEpochTime).count();
                if (milliseconds >= 500) {
                    nowEpochTime = currentEpochTime;
                    this->client->sendResponse(DtoWSTranscodeProgress::createProgressMessage());
                }

                error = outputContext.writePacket(packet);
                if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
                    std::cout << error.getMessage() << " " << error.getAVErrorMessage() << std::endl;
                    return false;
                }

                return true;
            });
            return true;
        });

        if (error.getType() == ff::AV_ERROR_TYPE::USER_STOP) {
            this->client->sendResponse(DtoWSTranscodeProgress::createStopMessage());
        } else if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            this->client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getAVErrorMessage()));
        } else {
            encoder.flush();
        }
    }
}  // namespace tr

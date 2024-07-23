#pragma once

#include <memory>

#include "dto/DtoRequestInterface.hpp"
#include "ffmpegpp.hpp"

namespace tr {
    class DtoWSTranscodeRequest;
    using DtoWSTranscodeRequestPtr = std::shared_ptr<DtoWSTranscodeRequest>;

    class DtoWSTranscodeRequest : public DtoRequestInterface<DtoWSTranscodeRequest> {
    public:
        explicit DtoWSTranscodeRequest() = default;
        virtual ~DtoWSTranscodeRequest() = default;

    public:
        virtual bool parseJson(const std::shared_ptr<Json::Value> json) override;

    public:
        std::string getTargetFile();
        std::string getOutputFile();

        // codec
        ff::VIDEO_CODEC getVideoCodec();
        ff::AUDIO_CODEC getAudioCodec();

        // video
        long long getVideoBitrate();
        int getVideoWidth();
        int getVideoHeight();
        int getVideoGopSize();

        // audio
        long long getAudioBitrate();
        int getAudioSampleRate();

    private:
        ff::VIDEO_CODEC parseVideoCodec(const std::string& codec);
        ff::AUDIO_CODEC parseAudioCodec(const std::string& codec);

    private:
        std::string targetFile;
        std::string outputFile;

        // Codec
        ff::VIDEO_CODEC videoCodec = ff::VIDEO_CODEC::NONE;
        ff::AUDIO_CODEC audioCodec = ff::AUDIO_CODEC::NONE;

        // video
        long long videoBitrate = 0L;
        int videoWidth = 0;
        int videoHeight = 0;
        int videoGopSize = 0;

        // Audio
        long long audioBitrate = 0L;
        int audioSampleRate = 0;
    };
}

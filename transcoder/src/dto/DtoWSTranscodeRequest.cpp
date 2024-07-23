#include "dto/DtoWSTranscodeRequest.hpp"

namespace tr {
    bool DtoWSTranscodeRequest::parseJson(const std::shared_ptr<Json::Value> json) {
        try {
            this->targetFile = (*json)["file"].asString();
            this->outputFile = (*json)["output"].asString();

            this->videoCodec = parseVideoCodec((*json)["video_codec"].asString());
            this->audioCodec = parseAudioCodec((*json)["audio_codec"].asString());

            this->videoBitrate = (*json)["video_bitrate"].asInt64();
            this->videoWidth = (*json)["video_width"].asInt();
            this->videoHeight = (*json)["video_height"].asInt();
            this->videoGopSize = (*json)["video_gop_size"].asInt();

            this->audioBitrate = (*json)["audio_bitrate"].asInt64();
            this->audioSampleRate = (*json)["audio_sample_rate"].asInt();
        } catch (Json::Exception& e) {
            return false;
        }
        return true;
    }

    std::string DtoWSTranscodeRequest::getTargetFile() {
        return this->targetFile;
    }

    std::string DtoWSTranscodeRequest::getOutputFile() {
        return this->outputFile;
    }

    ff::VIDEO_CODEC DtoWSTranscodeRequest::getVideoCodec() {
        return this->videoCodec;
    }

    ff::AUDIO_CODEC DtoWSTranscodeRequest::getAudioCodec() {
        return this->audioCodec;
    }

    long long DtoWSTranscodeRequest::getVideoBitrate() {
        return this->videoBitrate;
    }

    int DtoWSTranscodeRequest::getVideoWidth() {
        return this->videoWidth;
    }

    int DtoWSTranscodeRequest::getVideoHeight() {
        return this->videoHeight;
    }

    int DtoWSTranscodeRequest::getVideoGopSize() {
        return this->videoGopSize;
    }

    long long DtoWSTranscodeRequest::getAudioBitrate() {
        return this->audioBitrate;
    }

    int DtoWSTranscodeRequest::getAudioSampleRate() {
        return this->audioSampleRate;
    }

    ff::VIDEO_CODEC DtoWSTranscodeRequest::parseVideoCodec(const std::string& codec) {
        if (codec == "h264") {
            return ff::VIDEO_CODEC::H264;
        } else if (codec == "h265") {
            return ff::VIDEO_CODEC::H265;
        } else {
            return ff::VIDEO_CODEC::NONE;
        }
    }

    ff::AUDIO_CODEC DtoWSTranscodeRequest::parseAudioCodec(const std::string& codec) {
        if (codec == "ac3") {
            return ff::AUDIO_CODEC::AC3;
        } else if (codec == "aac") {
            return ff::AUDIO_CODEC::AAC;
        } else {
            return ff::AUDIO_CODEC::NONE;
        }
    }

}

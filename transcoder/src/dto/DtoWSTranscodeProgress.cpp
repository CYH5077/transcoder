#include "dto/DtoWSTranscodeProgress.hpp"

namespace tr {
    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createProgressMessage() {
        DtoWSTranscodeProgressPtr progress = std::make_shared<DtoWSTranscodeProgress>();
        progress->addJson("result", "progress");
        return progress;
    }

    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createFinishMessage() {
        DtoWSTranscodeProgressPtr finish = std::make_shared<DtoWSTranscodeProgress>();
        finish->addJson("result", "finish");
        return finish;
    }

    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createStopMessage() {
        DtoWSTranscodeProgressPtr stop = std::make_shared<DtoWSTranscodeProgress>();
        stop->addJson("result", "stop");
        return stop;
    }

    DtoWSTranscodeProgress::DtoWSTranscodeProgress() : DtoResponseInterface("transcode_progress") {}
};

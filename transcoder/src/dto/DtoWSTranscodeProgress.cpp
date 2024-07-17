#include "dto/DtoWSTranscodeProgress.hpp"

namespace tr {
    DtoWSTranscodeProgress::DtoWSTranscodeProgress() : DtoResponseInterface("transcode_progress") {}

    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createProgressMessage() {
        DtoWSTranscodeProgressPtr progress = std::make_shared<DtoWSTranscodeProgress>();
        progress->addJson("result", "progress");
        return progress;
    }

    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createWaitMessage(int number) {
        DtoWSTranscodeProgressPtr wait = std::make_shared<DtoWSTranscodeProgress>();
        wait->addJson("result", "wait");
        wait->addJson("number", std::to_string(number));
        return wait;
    }

    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createStartMessage() {
        DtoWSTranscodeProgressPtr start = std::make_shared<DtoWSTranscodeProgress>();
        start->addJson("result", "start");
        return start;
    }

    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createFinishMessage(const std::string& outputFile) {
        DtoWSTranscodeProgressPtr finish = std::make_shared<DtoWSTranscodeProgress>();
        finish->addJson("result", "finish");
        return finish;
    }

    DtoWSTranscodeProgressPtr DtoWSTranscodeProgress::createStopMessage() {
        DtoWSTranscodeProgressPtr stop = std::make_shared<DtoWSTranscodeProgress>();
        stop->addJson("result", "stop");
        return stop;
    }
};

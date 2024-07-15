#include "http/ws_recv_handler/WSTranscodeTask.hpp"

#include "dto/DtoWSErrorResponse.hpp"
#include "ffmpegpp.hpp"

namespace tr {
    WSTranscodeTask::WSTranscodeTask() : WSRequestTaskInterface("transcode") {}

    void WSTranscodeTask::task(WSClientPtr client, std::shared_ptr<Json::Value> json) {
        auto request = DtoWSTranscodeRequest::create();

        if (request->parseJson(json) == false) {
            client->sendResponse(DtoWSErrorResponse::createErrorMessage("Invalid request"));
            return;
        }

        std::string filepath = drogon::app().getUploadPath() + "/" + request->getTargetFile();
        std::shared_ptr<ff::FFAVInputContext> inputContext = std::make_shared<ff::FFAVInputContext>();
        ff::AVError error = inputContext->open(filepath);
        if (error.getType() != ff::AV_ERROR_TYPE::SUCCESS) {
            LOG_ERROR << "======================================================";
            LOG_ERROR << "Failed to open file Error Message: " << error.getMessage();
            LOG_ERROR << "AV Error message: " << error.getAVErrorMessage();
            LOG_ERROR << "Filepath: " << filepath;
            LOG_ERROR << "======================================================";
            client->sendResponse(DtoWSErrorResponse::createErrorMessage(error.getMessage()));
            return;
        }

        client->transcodeStart(inputContext, request);

        client->sendResponse(DtoWSTranscodeResponse::create());
    }
}

#include "http/ws_recv_handler/WSFileListTask.hpp"

#include "dto/DtoWSErrorResponse.hpp"
#include "server/Config.hpp"

namespace tr {
    WSFileListTask::WSFileListTask() : WSRequestTaskInterface("file_list") {}

    void WSFileListTask::task(WSClientPtr client, std::shared_ptr<Json::Value> json) {
        auto request = DtoWSFileListRequest::create();
        if (request->parseJson(json) == false) {
            client->sendResponse(DtoWSErrorResponse::createErrorMessage("Invalid request"));
            return;
        }

        std::string path = request->isTranscodedFileList() ? Config::getInstance().getTranscodeDir()
                                                           : Config::getInstance().getUploadDir();

        try {
            std::vector<std::string> fileList;
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (std::filesystem::is_regular_file(entry.status())) {
                    fileList.push_back(entry.path().filename().string());
                }
            }

            auto response = DtoWSFileListResponse::create();
            response->setFileList(fileList);
            client->sendResponse(response);

        } catch (const std::filesystem::filesystem_error& e) {
            client->sendResponse(DtoWSErrorResponse::createErrorMessage(e.what()));
        }
    }
}

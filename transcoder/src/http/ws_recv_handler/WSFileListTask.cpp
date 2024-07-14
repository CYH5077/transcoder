#include "http/ws_recv_handler/WSFileListTask.hpp"

#include "dto/DtoWSErrorResponse.hpp"
#include "dto/DtoWSFileListReuqest.hpp"
#include "dto/DtoWSFileListResponse.hpp"

namespace tr {
    WSFileListTask::WSFileListTask() : WSRequestTaskInterface("file_list") {}

    void WSFileListTask::task(WSClientPtr client, std::shared_ptr<Json::Value> json) {
        auto request = DtoWSFileListRequest::create();
        request->parseJson(json);

        std::string path = drogon::app().getUploadPath();
        std::vector<std::string> fileList;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (std::filesystem::is_regular_file(entry.status())) {
                    fileList.push_back(entry.path().filename().string());
                }
            }

            auto response = DtoWSFileListResponse::create();
            response->setFileList(fileList);
            client->sendResponse(response);

        } catch (const std::filesystem::filesystem_error& e) {
            auto response = DtoWSErrorResponse::create();
            client->sendResponse(response);
        }
    }
}

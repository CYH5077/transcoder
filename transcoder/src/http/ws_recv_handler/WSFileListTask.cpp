#include "http/ws_recv_handler/WSFileListTask.hpp"

namespace tr {
    WSFileListTask::WSFileListTask() : WSRequestTaskInterface("file_list") {}
    
    void WSFileListTask::task(WSClientPtr client, std::shared_ptr<Json::Value> json) {
        
    }
}

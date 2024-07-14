#include "http/ws_recv_handler/WSRequestTask.hpp"

namespace tr {
    WSRequestTask::WSRequestTask(const std::string& taskType) : taskType(taskType) {}

    std::string WSRequestTask::getTaskType() {
        return this->taskType;
    }
}

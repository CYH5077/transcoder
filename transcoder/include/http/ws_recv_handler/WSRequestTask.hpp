#pragma once

#include "drogon/drogon.h"
#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    class WSRequestTask;
    using WSRequestTaskPtr = std::shared_ptr<WSRequestTask>;

    class WSRequestTask {
    public:
        explicit WSRequestTask(const std::string& taskType);
        virtual ~WSRequestTask() = default;

    public:
        virtual void task(WSClientPtr client, std::shared_ptr<Json::Value> json) = 0;

    public:
        std::string getTaskType();

    private:
        std::string taskType;
    };
};

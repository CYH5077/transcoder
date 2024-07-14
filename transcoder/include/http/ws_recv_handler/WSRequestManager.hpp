#pragma once

#include <unordered_map>
#include <vector>

#include "drogon/drogon.h"
#include "http/ws_recv_handler/WSClient.hpp"
#include "http/ws_recv_handler/WSRequestFilter.hpp"
#include "http/ws_recv_handler/WSRequestTask.hpp"

namespace tr {
    class WSRequestManager {
    public:
        explicit WSRequestManager();
        virtual ~WSRequestManager() = default;

    public:
        void run(WSClientPtr client, std::shared_ptr<Json::Value> json);

    public:
        void initFilter();
        void initTask();

    private:
        void addFilter(WSRequestFilterPtr filter);
        void addTask(WSRequestTaskPtr task);

    private:
        std::unordered_map<std::string, WSRequestTaskPtr> tasks;
        std::vector<WSRequestFilterPtr> filters;
    };
};

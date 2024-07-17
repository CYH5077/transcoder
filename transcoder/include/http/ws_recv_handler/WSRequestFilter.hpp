#pragma once

#include "drogon/drogon.h"
#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    class WSRequestFilter;
    using WSRequestFilterPtr = std::shared_ptr<WSRequestFilter>;

    class WSRequestFilter {
    public:
        explicit WSRequestFilter() = default;
        virtual ~WSRequestFilter() = default;

    public:
        virtual bool filter(WSClientPtr client, std::shared_ptr<Json::Value> json) = 0;
    };
};

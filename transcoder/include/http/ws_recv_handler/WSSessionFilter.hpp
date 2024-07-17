#pragma once

#include "http/ws_recv_handler/WSRequestFilterInterface.hpp"
#include "http/ws_recv_handler/WSSessionFilter.hpp"

namespace tr {
    class WSSessionFilter : public WSRequestFilterInterface<WSSessionFilter> {
    public:
        explicit WSSessionFilter() = default;
        virtual ~WSSessionFilter() = default;

    public:
        virtual bool filter(WSClientPtr client, std::shared_ptr<Json::Value> json) override;
    };
}

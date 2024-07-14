#pragma once

#include "dto/DtoWSFileListResponse.hpp"
#include "dto/DtoWSFileListReuqest.hpp"
#include "http/ws_recv_handler/WSRequestTaskInterface.hpp"

namespace tr {
    class WSFileListTask : public WSRequestTaskInterface<WSFileListTask> {
    public:
        explicit WSFileListTask();
        virtual ~WSFileListTask() = default;

    public:
        virtual void task(WSClientPtr client, std::shared_ptr<Json::Value> json) override;

    private:
        DtoWSFileListRequest request;
        DtoWSFileListResponse response;
    };

    using WSFileListTaskPtr = std::shared_ptr<WSFileListTask>;
};

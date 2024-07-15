#pragma once

#include "dto/DtoWSTranscodeRequest.hpp"
#include "dto/DtoWSTranscodeResponse.hpp"
#include "http/ws_recv_handler/WSRequestTaskInterface.hpp"

namespace tr {
    class WSTranscodeTask : public WSRequestTaskInterface<WSTranscodeTask> {
    public:
        explicit WSTranscodeTask();
        virtual ~WSTranscodeTask() = default;

    public:
        virtual void task(WSClientPtr client, std::shared_ptr<Json::Value> json) override;

    };
}

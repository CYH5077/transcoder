#pragma once

#include <string>

#include "dto/DtoRequestInterface.hpp"

namespace tr {
    class DtoWSFileListRequest : DtoRequestInterface<DtoWSFileListRequest> {
    public:
        explicit DtoWSFileListRequest() = default;
        virtual ~DtoWSFileListRequest() = default;

    public:
        virtual void parseJson(const std::shared_ptr<Json::Value> json) override;
    };
};

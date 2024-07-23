#pragma once

#include <string>

#include "dto/DtoRequestInterface.hpp"

namespace tr {
    class DtoWSFileListRequest : public DtoRequestInterface<DtoWSFileListRequest> {
    public:
        explicit DtoWSFileListRequest() = default;
        virtual ~DtoWSFileListRequest() = default;

    public:
        virtual bool parseJson(const std::shared_ptr<Json::Value> json) override;

    public:
        bool isTranscodedFileList();

    private:
        bool _isTranscodedFileList;
    };
};

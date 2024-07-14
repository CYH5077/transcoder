#pragma once

#include <string>
#include <vector>

#include "dto/DtoResponseInterface.hpp"

namespace tr {
    class DtoWSFileListResponse : DtoResponseInterface<DtoWSFileListResponse> {
    public:
        explicit DtoWSFileListResponse() = default;
        virtual ~DtoWSFileListResponse() = default;

    public:
        void setFileList(std::vector<std::string>& fileList);
    };
};

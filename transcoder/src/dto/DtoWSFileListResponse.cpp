#include "dto/DtoWSFileListResponse.hpp"

namespace tr {
    DtoWSFileListResponse::DtoWSFileListResponse() : DtoResponseInterface("file_list") {}

    void DtoWSFileListResponse::setFileList(std::vector<std::string>& fileList) {
        DtoResponseInterface::addJsonArray("file_list", fileList);
    }
};

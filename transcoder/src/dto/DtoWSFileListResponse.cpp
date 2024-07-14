#include "dto/DtoWSFileListResponse.hpp"

namespace tr {
    void DtoWSFileListResponse::setFileList(std::vector<std::string>& fileList) {
        this->addJsonArray("file_list", fileList);
    }
};

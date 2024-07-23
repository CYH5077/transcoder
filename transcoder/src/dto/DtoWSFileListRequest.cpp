#include "dto/DtoWSFileListReuqest.hpp"

namespace tr {
    bool DtoWSFileListRequest::parseJson(const std::shared_ptr<Json::Value> json) {
        try {
            this->_isTranscodedFileList = (*json)["transcoded"].asBool();
        } catch (Json::Exception& e) {
            this->_isTranscodedFileList = false;
        }
        return true;
    }

    bool DtoWSFileListRequest::isTranscodedFileList() {
        return this->_isTranscodedFileList;
    }
};

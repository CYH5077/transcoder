#include "dto/DtoWSTranscodeRequest.hpp"

namespace tr {
    bool DtoWSTranscodeRequest::parseJson(const std::shared_ptr<Json::Value> json) {
        try {
            this->targetFile = (*json)["file"].asString();
		} catch (Json::Exception& e) {
			return false;
		}
        return true;
    }

    std::string DtoWSTranscodeRequest::getTargetFile() {
        return this->targetFile;
    }

}

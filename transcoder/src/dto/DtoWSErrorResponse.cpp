#include "dto/DtoWSErrorResponse.hpp"

namespace tr {
    DtoResponseJsonTypePtr DtoWSErrorResponse::createErrorMessage(const std::string& errorMessage) {
        return std::make_shared<DtoWSErrorResponse>(errorMessage);
    }

    DtoWSErrorResponse::DtoWSErrorResponse() : DtoResponseInterface("error") {}

    DtoWSErrorResponse::DtoWSErrorResponse(const std::string& errorMessage) : DtoResponseInterface("error") {
        this->setErrorMessage(errorMessage);
    }

    void DtoWSErrorResponse::setErrorMessage(const std::string& errorMessage) {
        DtoResponseInterface::addJson("error_message", errorMessage);
    }
}

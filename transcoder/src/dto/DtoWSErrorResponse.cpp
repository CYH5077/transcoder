#include "dto/DtoWSErrorResponse.hpp"

namespace tr {
    DtoWSErrorResponse::DtoWSErrorResponse() : DtoResponseInterface("error") {}

    void DtoWSErrorResponse::setErrorMessage(const std::string& errorMessage) {
        DtoResponseInterface::addJson("error_message", errorMessage);
    }
}

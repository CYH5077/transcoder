#pragma once

#include "dto/DtoResponseInterface.hpp"

namespace tr {
    class DtoWSErrorResponse : public DtoResponseInterface<DtoWSErrorResponse> {
    public:
        explicit DtoWSErrorResponse();
        virtual ~DtoWSErrorResponse() = default;

    public:
        void setErrorMessage(const std::string& errorMessage);
    };
};

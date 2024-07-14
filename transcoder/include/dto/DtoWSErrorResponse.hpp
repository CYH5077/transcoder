#pragma once

#include "dto/DtoResponseInterface.hpp"

namespace tr {
    class DtoWSErrorResponse : public DtoResponseInterface<DtoWSErrorResponse> {
    public:
        static DtoResponseJsonTypePtr createErrorMessage(const std::string& errorMessage);

    public:
        explicit DtoWSErrorResponse();
        explicit DtoWSErrorResponse(const std::string& errorMessage);
        virtual ~DtoWSErrorResponse() = default;

    public:
        void setErrorMessage(const std::string& errorMessage);
    };
};

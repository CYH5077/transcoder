#pragma once

#include "dto/DtoResponseInterface.hpp"

namespace tr {
    class DtoWSTranscodeResponse : public DtoResponseInterface<DtoWSTranscodeResponse> {
    public:
        explicit DtoWSTranscodeResponse();
        virtual ~DtoWSTranscodeResponse() = default;
    };
}

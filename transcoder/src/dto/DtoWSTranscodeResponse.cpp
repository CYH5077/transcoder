#include "dto/DtoWSTranscodeResponse.hpp"

namespace tr {
    DtoWSTranscodeResponse::DtoWSTranscodeResponse() : DtoResponseInterface("transcode") {
        DtoResponseInterface::addJson("result", "start");
    }
};

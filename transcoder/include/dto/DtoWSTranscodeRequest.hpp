#pragma once

#include "dto/DtoRequestInterface.hpp"

#include <memory>

namespace tr {
    class DtoWSTranscodeRequest;
    using DtoWSTranscodeRequestPtr = std::shared_ptr<DtoWSTranscodeRequest>;

    class DtoWSTranscodeRequest : public DtoRequestInterface<DtoWSTranscodeRequest> {
    public:
        explicit DtoWSTranscodeRequest() = default;
        virtual ~DtoWSTranscodeRequest() = default;

    public:
        virtual bool parseJson(const std::shared_ptr<Json::Value> json) override;

    public:
        std::string getTargetFile();
        std::string getOutputFile();

    private:
        std::string targetFile;
        std::string outputFile;
    };
}

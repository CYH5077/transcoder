#pragma once

#include "dto/DtoResponseInterface.hpp"

namespace tr {
    class DtoConnectMessage;
    using DtoConnectMessagePtr = std::shared_ptr<DtoConnectMessage>;

    class DtoConnectMessage : public DtoResponseInterface<DtoConnectMessage> {
    public:
        static DtoConnectMessagePtr create(const std::string& sessionId);

    public:
        explicit DtoConnectMessage() = default;
        explicit DtoConnectMessage(const std::string& sessionId);
        virtual ~DtoConnectMessage() = default;
    };
};

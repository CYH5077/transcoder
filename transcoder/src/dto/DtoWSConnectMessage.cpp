#include "dto/DtoWSConnectMessage.hpp"

namespace tr {
    DtoConnectMessagePtr DtoConnectMessage::create(const std::string& sessionId) {
        return std::make_shared<DtoConnectMessage>(sessionId);
    }

    DtoConnectMessage::DtoConnectMessage(const std::string& sessionId) : DtoResponseInterface("connect") {
        this->addJson("session_id", sessionId);
    }
}

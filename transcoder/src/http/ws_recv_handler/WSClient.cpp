#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    WSClientPtr WSClient::create(drogon::WebSocketConnectionPtr conn) {
        return std::make_shared<WSClient>(conn);
    }

    WSClient::WSClient(drogon::WebSocketConnectionPtr conn) : conn(conn), state(TRANSCODE_STATE::NONE) {}

    void WSClient::setConnect(bool connected) {
        this->connected = connected;
    }

    bool WSClient::isConnected() {
		return this->connected;
	}

    void WSClient::setTranscodeState(TRANSCODE_STATE state) {
        this->state = state;
    }

    TRANSCODE_STATE WSClient::getTranscodeState() {
		return this->state;
	}

    void WSClient::sendResponse(DtoResponseJsonTypePtr dto) {
		if (this->connected) {
			this->conn->sendJson(dto->toJson());
		}
	}
}

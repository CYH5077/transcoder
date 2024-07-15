#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    WSClientPtr WSClient::create(drogon::WebSocketConnectionPtr conn) {
        auto client = std::make_shared<WSClient>(conn);
        client->setTranscodeThread(TranscodeThread::create(client));
        return client;
    }

    WSClient::WSClient(drogon::WebSocketConnectionPtr conn) : conn(conn) {}

    void WSClient::setConnect(bool connected) {
        this->connected = connected;
    }

    bool WSClient::isConnected() {
		return this->connected;
	}

    void WSClient::sendResponse(DtoResponseJsonTypePtr dto) {
		if (this->connected) {
			this->conn->sendJson(dto->toJson());
		}
	}

    void WSClient::setTranscodeThread(TranscodeThreadPtr transcodeThread) {
        this->transcodeThread = transcodeThread;
    }

    void WSClient::transcodeStart(std::shared_ptr<ff::FFAVInputContext> inputContext,
                                  DtoWSTranscodeRequestPtr request) {
        this->transcodeThread->start(inputContext, request);
    }

    void WSClient::stopAll() {
        this->transcodeThread->stop();
    }
}

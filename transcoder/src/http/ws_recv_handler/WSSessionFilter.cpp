#include "http/ws_recv_handler/WSSessionFilter.hpp"

namespace tr {
    bool WSSessionFilter::filter(WSClientPtr client, std::shared_ptr<Json::Value> json) {
        if ((*json)["session_id"] != client->getSessionId()) {
            return false;
        }
        return true;
    }
}

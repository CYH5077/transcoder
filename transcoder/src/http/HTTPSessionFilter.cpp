#include "http/HTTPSessionFilter.hpp"

#include "http/HTTPSessionManager.hpp"

namespace tr {
    void HTTPSessionFilter::doFilter(const drogon::HttpRequestPtr& req,
                                     drogon::FilterCallback&& fcb,
                                     drogon::FilterChainCallback&& fccb) {
        std::string sessionId = req->getHeader("TRSession");
        WSClientPtr client = HTTPSessionManager::getInstance().getClient(sessionId);
        if (client != nullptr) {
            fccb();
		} else {
			auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::HttpStatusCode::k403Forbidden);
            fcb(resp);
		}
    }
}

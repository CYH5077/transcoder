#pragma once

#include "drogon/HttpFilter.h"
#include "drogon/drogon.h"

namespace tr {
    class HTTPSessionFilter : public drogon::HttpFilter<HTTPSessionFilter> {
    public:
        virtual void doFilter(const drogon::HttpRequestPtr& req,
                              drogon::FilterCallback&& fcb,
                              drogon::FilterChainCallback&& fccb) override;
    };
}

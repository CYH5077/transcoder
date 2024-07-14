#pragma once

#include "http/ws_recv_handler/WSRequestFilter.hpp"

namespace tr {
    template <typename T>
    class WSRequestFilterInterface : public WSRequestFilter {
    public:
        static std::shared_ptr<T> create() {
            return std::make_shared<T>();
        }

    public:
        explicit WSRequestFilterInterface() = default;
        virtual ~WSRequestFilterInterface() = default;
    };
};

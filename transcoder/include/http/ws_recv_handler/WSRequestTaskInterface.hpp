#pragma once

#include <memory>

#include "http/ws_recv_handler/WSRequestTask.hpp"

namespace tr {
    template <typename T>
    class WSRequestTaskInterface : public WSRequestTask {
    public:
        static std::shared_ptr<T> create() {
            return std::make_shared<T>();
        }

    public:
        explicit WSRequestTaskInterface(const std::string& taskType) : WSRequestTask(taskType) {}
        virtual ~WSRequestTaskInterface() = default;
    };
};

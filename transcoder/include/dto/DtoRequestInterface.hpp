#pragma once

#include "drogon/drogon.h"

#include <memory>

namespace tr {
    template <typename T>
    class DtoRequestInterface {
    public:
        static std::shared_ptr<T> create() {
            return std::make_shared<T>();
        }

    public:
        explicit DtoRequestInterface() = default;
        virtual ~DtoRequestInterface() = default;

    public:
        virtual void parseJson(const std::shared_ptr<Json::Value> json) = 0;
    };
}

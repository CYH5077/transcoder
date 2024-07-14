#pragma once

#include <memory>

#include "drogon/drogon.h"
#include "dto/DtoResponseJsonType.hpp"

namespace tr {
    template <typename T>
    class DtoResponseInterface : public DtoResponseJsonType {
    public:
        static std::shared_ptr<T> create() {
            return std::make_shared<T>();
        }

    public:
        explicit DtoResponseInterface(const std::string& responseType) : DtoResponseJsonType(responseType) {}
        virtual ~DtoResponseInterface() = default;
    };
};

#pragma once

#include <memory>

#include "drogon/drogon.h"

namespace tr {
    class DtoResponseJsonType;
    using DtoResponseJsonTypePtr = std::shared_ptr<DtoResponseJsonType>;

    class DtoResponseJsonType {
    public:
        explicit DtoResponseJsonType(const std::string& responseType);
        virtual ~DtoResponseJsonType() = default;

    public:
        const Json::Value& toJson();

    protected:
        void addJson(const std::string& key, const std::string& value);
        void addJsonArray(const std::string& key, const std::vector<std::string>& value);
        void addJsonArray(const std::string& key, const std::vector<int>& value);
        void addJsonObject(const std::string& key, const Json::Value& value);

    private:
        std::string responseType;

        Json::Value json;
    };
};

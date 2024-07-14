#pragma once

#include "drogon/drogon.h"

namespace tr {
    class DtoResponseJsonType {
    public:
        explicit DtoResponseJsonType() = default;
        virtual ~DtoResponseJsonType() = default;

    public:
        Json::Value toJson();

    protected:
        void addJson(const std::string& key, const std::string& value);
        void addJsonArray(const std::string& key, const std::vector<std::string>& value);
        void addJsonArray(const std::string& key, const std::vector<int>& value);
        void addJsonObject(const std::string& key, const Json::Value& value);

    private:
        Json::Value json;
    };
};

#include "dto/DtoResponseJsonType.hpp"

namespace tr {
    DtoResponseJsonType::DtoResponseJsonType(const std::string& responseType) {
		this->json["type"] = responseType;
	}

    void DtoResponseJsonType::addJson(const std::string& key, const std::string& value) {
        this->json[key] = value;
    }

    void DtoResponseJsonType::addJsonArray(const std::string& key, const std::vector<std::string>& value) {
        this->json[key] = Json::Value(Json::arrayValue);
        for (const auto& v : value) {
            this->json[key].append(v);
        }
    }

    void DtoResponseJsonType::addJsonArray(const std::string& key, const std::vector<int>& value) {
        for (const auto& v : value) {
            this->json[key].append(v);
        }
    }

    void DtoResponseJsonType::addJsonObject(const std::string& key, const Json::Value& value) {
        this->json[key] = value;
    }

    const Json::Value& DtoResponseJsonType::toJson() {
        return this->json;
    }
}

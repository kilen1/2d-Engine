#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace CmakeProject1 {

class Serializer {
public:
    static nlohmann::json serializeObject(const std::string& objectType, void* object);
    static void deserializeObject(const nlohmann::json& j, const std::string& objectType, void* object);
    
    template<typename T>
    static nlohmann::json toJson(const T& obj) {
        return nlohmann::json(obj);
    }
    
    template<typename T>
    static void fromJson(const nlohmann::json& j, T& obj) {
        j.get_to(obj);
    }
};

} // namespace CmakeProject1
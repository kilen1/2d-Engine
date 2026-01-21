#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace CmakeProject1 {

class Config {
public:
    static Config& getInstance();
    
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);
    
    template<typename T>
    T getValue(const std::string& key, const T& defaultValue = T{}) {
        // Implementation would depend on config storage
        return defaultValue;
    }
    
    template<typename T>
    void setValue(const std::string& key, const T& value) {
        // Implementation would depend on config storage
    }

private:
    Config() = default;
    ~Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};

} // namespace CmakeProject1
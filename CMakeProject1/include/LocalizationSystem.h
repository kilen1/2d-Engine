#pragma once
#include <string>
#include <unordered_map>

namespace CmakeProject1 {

class LocalizationSystem {
public:
    static LocalizationSystem& getInstance();
    
    bool loadLanguage(const std::string& languageCode, const std::string& filepath);
    std::string getText(const std::string& key, const std::string& languageCode = "");
    void setCurrentLanguage(const std::string& languageCode);
    std::string getCurrentLanguage() const;
    void reloadTranslations();

private:
    LocalizationSystem() = default;
    ~LocalizationSystem() = default;
    LocalizationSystem(const LocalizationSystem&) = delete;
    LocalizationSystem& operator=(const LocalizationSystem&) = delete;
    
    std::string currentLanguage;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations;
};

} // namespace CmakeProject1
#include "../include/LocalizationManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace CmakeProject1 {

    LocalizationManager& LocalizationManager::getInstance() {
        static LocalizationManager instance;
        return instance;
    }

    LocalizationManager::LocalizationManager() : currentLanguage("en"), fallbackLanguage("en"), initialized(false) {
        // Initialize localization manager
    }

    LocalizationManager::~LocalizationManager() {
        // Clean up localization resources
    }

    bool LocalizationManager::Initialize() {
        initialized = true;
        // Additional initialization logic here
        return true;
    }

    void LocalizationManager::LoadLanguage(const std::string& languageCode) {
        // Load specific language file
        std::string filePath = "Assets/Locales/" + languageCode + ".json";
        LoadFromFile(filePath);
    }

    void LocalizationManager::LoadFromFile(const std::string& filePath) {
        // Load localization data from file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return;
        }
        
        // Parse file and populate localizationData
        // Implementation depends on file format (JSON, XML, etc.)
    }

    void LocalizationManager::LoadFromDirectory(const std::string& directoryPath) {
        // Load all localization files from directory
    }

    std::string LocalizationManager::GetText(const std::string& key) const {
        return GetText(key, key); // Return key as default if not found
    }

    std::string LocalizationManager::GetText(const std::string& key, const std::string& defaultValue) const {
        auto langIt = localizationData.find(currentLanguage);
        if (langIt != localizationData.end()) {
            auto textIt = langIt->second.find(key);
            if (textIt != langIt->second.end()) {
                return textIt->second;
            }
        }
        
        // Try fallback language
        if (currentLanguage != fallbackLanguage) {
            auto fallbackIt = localizationData.find(fallbackLanguage);
            if (fallbackIt != localizationData.end()) {
                auto textIt = fallbackIt->second.find(key);
                if (textIt != fallbackIt->second.end()) {
                    return textIt->second;
                }
            }
        }
        
        return defaultValue;
    }

    std::string LocalizationManager::GetPluralText(const std::string& key, int count, const std::string& defaultValue) const {
        std::string pluralKey = key + "_" + std::to_string(count);
        return GetText(pluralKey, defaultValue);
    }

    std::vector<std::string> LocalizationManager::GetAvailableLanguages() const {
        std::vector<std::string> languages;
        for (const auto& pair : localizationData) {
            languages.push_back(pair.first);
        }
        return languages;
    }

    std::string LocalizationManager::GetCurrentLanguage() const {
        return currentLanguage;
    }

    void LocalizationManager::SetCurrentLanguage(const std::string& languageCode) {
        if (localizationData.find(languageCode) != localizationData.end()) {
            currentLanguage = languageCode;
        }
    }

    bool LocalizationManager::HasLanguage(const std::string& languageCode) const {
        return localizationData.find(languageCode) != localizationData.end();
    }

    std::string LocalizationManager::Format(const std::string& key, const std::vector<std::string>& args) const {
        std::string text = GetText(key);
        for (size_t i = 0; i < args.size(); ++i) {
            std::string placeholder = "{" + std::to_string(i) + "}";
            size_t pos = 0;
            while ((pos = text.find(placeholder, pos)) != std::string::npos) {
                text.replace(pos, placeholder.length(), args[i]);
                pos += args[i].length();
            }
        }
        return text;
    }

    std::string LocalizationManager::Format(const std::string& key, const std::unordered_map<std::string, std::string>& args) const {
        std::string text = GetText(key);
        for (const auto& argPair : args) {
            std::string placeholder = "{" + argPair.first + "}";
            size_t pos = 0;
            while ((pos = text.find(placeholder, pos)) != std::string::npos) {
                text.replace(pos, placeholder.length(), argPair.second);
                pos += argPair.second.length();
            }
        }
        return text;
    }

    void LocalizationManager::AddTranslation(const std::string& languageCode, const std::string& key, const std::string& value) {
        localizationData[languageCode][key] = value;
    }

    void LocalizationManager::AddTranslations(const std::string& languageCode, const std::unordered_map<std::string, std::string>& translations) {
        for (const auto& pair : translations) {
            localizationData[languageCode][pair.first] = pair.second;
        }
    }

    void LocalizationManager::ReloadCurrentLanguage() {
        LoadLanguage(currentLanguage);
    }

    void LocalizationManager::ClearCache() {
        localizationData.clear();
    }

    void LocalizationManager::UnloadLanguage(const std::string& languageCode) {
        localizationData.erase(languageCode);
    }

    bool LocalizationManager::HasTranslation(const std::string& key) const {
        auto langIt = localizationData.find(currentLanguage);
        if (langIt != localizationData.end()) {
            return langIt->second.find(key) != langIt->second.end();
        }
        return false;
    }

    std::string LocalizationManager::GetFallbackLanguage() const {
        return fallbackLanguage;
    }

    void LocalizationManager::SetFallbackLanguage(const std::string& languageCode) {
        fallbackLanguage = languageCode;
    }

}
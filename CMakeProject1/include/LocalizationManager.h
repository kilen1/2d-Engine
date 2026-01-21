#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace CmakeProject1 {

    class LocalizationManager {
    public:
        static LocalizationManager& getInstance();

        // Initialization and configuration
        bool Initialize();
        void LoadLanguage(const std::string& languageCode);
        void LoadFromFile(const std::string& filePath);
        void LoadFromDirectory(const std::string& directoryPath);

        // Text translation
        std::string GetText(const std::string& key) const;
        std::string GetText(const std::string& key, const std::string& defaultValue) const;
        std::string GetPluralText(const std::string& key, int count, const std::string& defaultValue = "") const;

        // Language management
        std::vector<std::string> GetAvailableLanguages() const;
        std::string GetCurrentLanguage() const;
        void SetCurrentLanguage(const std::string& languageCode);
        bool HasLanguage(const std::string& languageCode) const;

        // String formatting with localization
        std::string Format(const std::string& key, const std::vector<std::string>& args) const;
        std::string Format(const std::string& key, const std::unordered_map<std::string, std::string>& args) const;

        // Adding translations programmatically
        void AddTranslation(const std::string& languageCode, const std::string& key, const std::string& value);
        void AddTranslations(const std::string& languageCode, const std::unordered_map<std::string, std::string>& translations);

        // Resource management
        void ReloadCurrentLanguage();
        void ClearCache();
        void UnloadLanguage(const std::string& languageCode);

        // Utility methods
        bool HasTranslation(const std::string& key) const;
        std::string GetFallbackLanguage() const;
        void SetFallbackLanguage(const std::string& languageCode);

    private:
        LocalizationManager();
        ~LocalizationManager();

        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> localizationData;
        std::string currentLanguage;
        std::string fallbackLanguage = "en";
        std::vector<std::string> availableLanguages;
        bool initialized = false;
    };

} // namespace CmakeProject1
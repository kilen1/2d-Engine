#pragma once

namespace CmakeProject1 {

class AccessibilitySystem {
public:
    static AccessibilitySystem& getInstance();
    
    void enableSubtitles(bool enabled);
    void setSubtitleSize(int size);
    void setSubtitleColor(unsigned int color);
    void enableHighContrast(bool enabled);
    void enableColorblindMode(bool enabled);
    void setUIScale(float scale);
    void enableScreenReader(bool enabled);
    void updateAccessibility();

private:
    AccessibilitySystem() = default;
    ~AccessibilitySystem() = default;
    AccessibilitySystem(const AccessibilitySystem&) = delete;
    AccessibilitySystem& operator=(const AccessibilitySystem&) = delete;
    
    bool subtitlesEnabled;
    int subtitleSize;
    unsigned int subtitleColor;
    bool highContrastEnabled;
    bool colorblindModeEnabled;
    float uiScale;
    bool screenReaderEnabled;
};

} // namespace CmakeProject1
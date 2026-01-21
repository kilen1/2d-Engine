#pragma once

namespace CmakeProject1 {

class ReflectionSystem {
public:
    static ReflectionSystem& getInstance();
    
    void enableReflections(bool enabled);
    void setReflectionQuality(int quality); // 0-3 low to ultra
    void updateReflections();
    void renderReflections();

private:
    ReflectionSystem() = default;
    ~ReflectionSystem() = default;
    ReflectionSystem(const ReflectionSystem&) = delete;
    ReflectionSystem& operator=(const ReflectionSystem&) = delete;
    
    bool reflectionsEnabled;
    int reflectionQuality;
};

} // namespace CmakeProject1
#pragma once

namespace CmakeProject1 {

class ShadowSystem {
public:
    static ShadowSystem& getInstance();
    
    void enableShadows(bool enabled);
    void setCascadeCount(int count);
    void updateShadows();
    void renderShadows();

private:
    ShadowSystem() = default;
    ~ShadowSystem() = default;
    ShadowSystem(const ShadowSystem&) = delete;
    ShadowSystem& operator=(const ShadowSystem&) = delete;
    
    bool shadowsEnabled;
    int cascadeCount;
};

} // namespace CmakeProject1
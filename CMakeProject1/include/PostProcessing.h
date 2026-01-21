#pragma once

namespace CmakeProject1 {

class PostProcessing {
public:
    static PostProcessing& getInstance();
    
    void enableBloom(bool enabled);
    void enableMotionBlur(bool enabled);
    void enableDepthOfField(bool enabled);
    void applyEffects();
    void render();

private:
    PostProcessing() = default;
    ~PostProcessing() = default;
    PostProcessing(const PostProcessing&) = delete;
    PostProcessing& operator=(const PostProcessing&) = delete;
    
    bool bloomEnabled;
    bool motionBlurEnabled;
    bool depthOfFieldEnabled;
};

} // namespace CmakeProject1
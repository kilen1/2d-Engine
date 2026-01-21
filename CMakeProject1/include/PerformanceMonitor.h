#pragma once

namespace CmakeProject1 {

class PerformanceMonitor {
public:
    static PerformanceMonitor& getInstance();
    
    void startFrame();
    void endFrame();
    float getFPS() const;
    float getFrameTime() const;
    void logPerformance();
    void enableProfiling(bool enabled);

private:
    PerformanceMonitor() = default;
    ~PerformanceMonitor() = default;
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    PerformanceMonitor& operator=(const PerformanceMonitor&) = delete;
    
    float fps;
    float frameTime;
    bool profilingEnabled;
};

} // namespace CmakeProject1
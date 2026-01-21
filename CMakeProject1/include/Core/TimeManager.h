#pragma once

#include <chrono>

namespace CmakeProject1 {

    class TimeManager {
    public:
        TimeManager();
        ~TimeManager();

        void update();
        
        float getDeltaTime() const { return deltaTime; }
        float getTimeScale() const { return timeScale; }
        void setTimeScale(float scale) { timeScale = scale; }
        
        float getTotalTime() const { return totalTime; }
        int getFPS() const { return fps; }
        
        void reset();

    private:
        std::chrono::steady_clock::time_point currentTime;
        std::chrono::steady_clock::time_point lastTime;
        float deltaTime = 0.0f;
        float totalTime = 0.0f;
        float timeScale = 1.0f;
        int frameCount = 0;
        int fps = 0;
        std::chrono::steady_clock::time_point fpsTimer;
    };

} // namespace CmakeProject1
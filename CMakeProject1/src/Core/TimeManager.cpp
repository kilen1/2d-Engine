#include "../../include/Core/TimeManager.h"
#include <SDL2/SDL.h>

namespace CmakeProject1 {

    TimeManager::TimeManager() {
        currentTime = std::chrono::steady_clock::now();
        lastTime = currentTime;
        fpsTimer = currentTime;
    }

    TimeManager::~TimeManager() {}

    void TimeManager::update() {
        currentTime = std::chrono::steady_clock::now();
        
        // Calculate delta time
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime);
        deltaTime = duration.count() / 1000000.0f;  // Convert to seconds
        deltaTime *= timeScale;  // Apply time scale
        
        // Update total time
        totalTime += deltaTime;
        
        // Update FPS counter
        frameCount++;
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - fpsTimer);
        if (elapsed.count() >= 1000) {  // Every second
            fps = frameCount;
            frameCount = 0;
            fpsTimer = currentTime;
        }
        
        lastTime = currentTime;
    }

    void TimeManager::reset() {
        totalTime = 0.0f;
        frameCount = 0;
        fps = 0;
        currentTime = std::chrono::steady_clock::now();
        lastTime = currentTime;
        fpsTimer = currentTime;
    }

} // namespace CmakeProject1
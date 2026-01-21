#pragma once

namespace CmakeProject1 {

class DayNightSystem {
public:
    static DayNightSystem& getInstance();
    
    void setTimeOfDay(float hours); // 0-24
    float getTimeOfDay() const;
    float getSunPosition() const; // 0-1 normalized
    void update(float deltaTime);
    void render();

private:
    DayNightSystem() = default;
    ~DayNightSystem() = default;
    DayNightSystem(const DayNightSystem&) = delete;
    DayNightSystem& operator=(const DayNightSystem&) = delete;
    
    float currentTime;
    float dayLength; // in seconds
};

} // namespace CmakeProject1
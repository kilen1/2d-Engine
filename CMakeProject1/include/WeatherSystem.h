#pragma once
#include <string>

namespace CmakeProject1 {

class WeatherSystem {
public:
    static WeatherSystem& getInstance();
    
    enum class WeatherType {
        Sunny,
        Rainy,
        Snowy,
        Stormy
    };
    
    void setWeather(WeatherType type);
    WeatherType getCurrentWeather() const;
    void update(float deltaTime);
    void render();

private:
    WeatherSystem() = default;
    ~WeatherSystem() = default;
    WeatherSystem(const WeatherSystem&) = delete;
    WeatherSystem& operator=(const WeatherSystem&) = delete;
    
    WeatherType currentWeather;
    float weatherIntensity;
};

} // namespace CmakeProject1
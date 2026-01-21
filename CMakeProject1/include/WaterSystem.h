#pragma once

namespace CmakeProject1 {

class WaterSystem {
public:
    static WaterSystem& getInstance();
    
    struct WaterBody {
        float x, y, width, height;
        float waveHeight;
        float waveSpeed;
    };
    
    void addWaterBody(float x, float y, float width, float height);
    void updateWaves(float deltaTime);
    void renderWater();
    void clearWaterBodies();

private:
    WaterSystem() = default;
    ~WaterSystem() = default;
    WaterSystem(const WaterSystem&) = delete;
    WaterSystem& operator=(const WaterSystem&) = delete;
    
    std::vector<WaterBody> waterBodies;
};

} // namespace CmakeProject1
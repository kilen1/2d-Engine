#pragma once
#include <vector>
#include <memory>

namespace CmakeProject1 {

class TerrainSystem {
public:
    static TerrainSystem& getInstance();
    
    struct TerrainChunk {
        int x, y;
        std::vector<float> heightmap;
        // Additional terrain data
    };
    
    bool generateTerrain(int width, int height);
    void updateTerrain();
    void renderTerrain();
    void clearTerrain();

private:
    TerrainSystem() = default;
    ~TerrainSystem() = default;
    TerrainSystem(const TerrainSystem&) = delete;
    TerrainSystem& operator=(const TerrainSystem&) = delete;
    
    std::vector<std::shared_ptr<TerrainChunk>> chunks;
};

} // namespace CmakeProject1
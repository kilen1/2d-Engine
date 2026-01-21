#pragma once
#include <string>
#include <vector>

namespace CmakeProject1 {

class CutsceneSystem {
public:
    static CutsceneSystem& getInstance();
    
    struct CutsceneAction {
        std::string type; // dialogue, camera_move, etc.
        float time;
        std::string data;
    };
    
    bool loadCutscene(const std::string& cutsceneId, const std::string& filepath);
    void playCutscene(const std::string& cutsceneId);
    void stopCutscene();
    void pauseCutscene();
    void resumeCutscene();
    bool isPlaying() const;
    void skipCutscene();

private:
    CutsceneSystem() = default;
    ~CutsceneSystem() = default;
    CutsceneSystem(const CutsceneSystem&) = delete;
    CutsceneSystem& operator=(const CutsceneSystem&) = delete;
    
    std::vector<CutsceneAction> currentActions;
    float currentTime;
    bool playing;
};

} // namespace CmakeProject1
#pragma once

namespace CmakeProject1 {

class LevelingSystem {
public:
    static LevelingSystem& getInstance();
    
    void setLevel(int level);
    int getLevel() const;
    float getExperience() const;
    float getExperienceForNextLevel() const;
    void gainExperience(float exp);
    bool canLevelUp() const;
    void levelUp();
    void update();
    
    static float calculateExpForLevel(int level);

private:
    LevelingSystem() = default;
    ~LevelingSystem() = default;
    LevelingSystem(const LevelingSystem&) = delete;
    LevelingSystem& operator=(const LevelingSystem&) = delete;
    
    int currentLevel;
    float currentExperience;
    float expForNextLevel;
};

} // namespace CmakeProject1
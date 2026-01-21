#pragma once
#include <string>
#include <vector>

namespace CmakeProject1 {

class AchievementSystem {
public:
    static AchievementSystem& getInstance();
    
    struct Achievement {
        std::string id;
        std::string title;
        std::string description;
        bool unlocked;
        std::string unlockCondition;
    };
    
    void registerAchievement(const Achievement& achievement);
    void unlockAchievement(const std::string& achievementId);
    void checkAchievements();
    const std::vector<Achievement>& getAchievements() const;
    bool isUnlocked(const std::string& achievementId) const;

private:
    AchievementSystem() = default;
    ~AchievementSystem() = default;
    AchievementSystem(const AchievementSystem&) = delete;
    AchievementSystem& operator=(const AchievementSystem&) = delete;
    
    std::vector<Achievement> achievements;
};

} // namespace CmakeProject1
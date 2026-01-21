#pragma once

namespace CmakeProject1 {

class SkillSystem {
public:
    static SkillSystem& getInstance();
    
    struct Skill {
        std::string id;
        std::string name;
        int level;
        int maxLevel;
        float experience;
        float experienceToNextLevel;
    };
    
    bool addSkill(const std::string& skillId, const std::string& name);
    bool upgradeSkill(const std::string& skillId);
    Skill* getSkill(const std::string& skillId);
    void gainExperience(const std::string& skillId, float exp);
    void resetSkills();
    const std::vector<Skill>& getAllSkills() const;

private:
    SkillSystem() = default;
    ~SkillSystem() = default;
    SkillSystem(const SkillSystem&) = delete;
    SkillSystem& operator=(const SkillSystem&) = delete;
    
    std::vector<Skill> skills;
};

} // namespace CmakeProject1
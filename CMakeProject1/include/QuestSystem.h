#pragma once
#include <string>
#include <vector>

namespace CmakeProject1 {

class QuestSystem {
public:
    static QuestSystem& getInstance();
    
    enum class QuestState {
        NotStarted,
        InProgress,
        Completed,
        Failed
    };
    
    struct Quest {
        std::string id;
        std::string title;
        std::string description;
        QuestState state;
        std::vector<std::string> objectives;
    };
    
    void addQuest(const Quest& quest);
    void updateQuest(const std::string& questId, QuestState newState);
    Quest* getQuest(const std::string& questId);
    void completeObjective(const std::string& questId, const std::string& objective);
    void clearQuests();

private:
    QuestSystem() = default;
    ~QuestSystem() = default;
    QuestSystem(const QuestSystem&) = delete;
    QuestSystem& operator=(const QuestSystem&) = delete;
    
    std::vector<Quest> quests;
};

} // namespace CmakeProject1
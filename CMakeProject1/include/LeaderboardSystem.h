#pragma once
#include <string>
#include <vector>

namespace CmakeProject1 {

class LeaderboardSystem {
public:
    static LeaderboardSystem& getInstance();
    
    struct ScoreEntry {
        std::string playerName;
        int score;
        std::string timestamp;
    };
    
    bool submitScore(const std::string& leaderboardId, const std::string& playerName, int score);
    std::vector<ScoreEntry> getTopScores(const std::string& leaderboardId, int count = 10);
    int getPlayerRank(const std::string& leaderboardId, const std::string& playerName);
    void refreshLeaderboards();

private:
    LeaderboardSystem() = default;
    ~LeaderboardSystem() = default;
    LeaderboardSystem(const LeaderboardSystem&) = delete;
    LeaderboardSystem& operator=(const LeaderboardSystem&) = delete;
    
    std::unordered_map<std::string, std::vector<ScoreEntry>> leaderboards;
};

} // namespace CmakeProject1
#pragma once
#include <string>

namespace CmakeProject1 {

class MatchmakingSystem {
public:
    static MatchmakingSystem& getInstance();
    
    enum class MatchType {
        QuickMatch,
        Ranked,
        Custom,
        Tournament
    };
    
    bool findMatch(MatchType type, int playerCount = 2);
    void cancelSearch();
    bool isInQueue() const;
    void reportResult(bool won);
    void update();

private:
    MatchmakingSystem() = default;
    ~MatchmakingSystem() = default;
    MatchmakingSystem(const MatchmakingSystem&) = delete;
    MatchmakingSystem& operator=(const MatchmakingSystem&) = delete;
    
    bool searching;
    MatchType currentMatchType;
    int targetPlayerCount;
};

} // namespace CmakeProject1
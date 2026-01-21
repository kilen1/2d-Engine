#pragma once
#include <string>

namespace CmakeProject1 {

class CheckpointSystem {
public:
    static CheckpointSystem& getInstance();
    
    bool setCheckpoint(const std::string& checkpointId);
    bool loadCheckpoint(const std::string& checkpointId);
    void setCurrentCheckpoint(const std::string& checkpointId);
    std::string getCurrentCheckpoint() const;
    void clearCheckpoints();

private:
    CheckpointSystem() = default;
    ~CheckpointSystem() = default;
    CheckpointSystem(const CheckpointSystem&) = delete;
    CheckpointSystem& operator=(const CheckpointSystem&) = delete;
    
    std::string currentCheckpoint;
};

} // namespace CmakeProject1
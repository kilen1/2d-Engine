#pragma once
#include <string>

namespace CmakeProject1 {

class SaveSystem {
public:
    static SaveSystem& getInstance();
    
    bool saveGame(const std::string& slotName);
    bool loadGame(const std::string& slotName);
    bool deleteSave(const std::string& slotName);
    bool saveExists(const std::string& slotName);
    void listSaves(std::vector<std::string>& outSaves);
    void autosave();

private:
    SaveSystem() = default;
    ~SaveSystem() = default;
    SaveSystem(const SaveSystem&) = delete;
    SaveSystem& operator=(const SaveSystem&) = delete;
};

} // namespace CmakeProject1
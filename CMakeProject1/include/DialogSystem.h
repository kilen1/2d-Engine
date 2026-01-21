#pragma once
#include <string>
#include <functional>

namespace CmakeProject1 {

class DialogSystem {
public:
    static DialogSystem& getInstance();
    
    struct DialogNode {
        std::string text;
        std::vector<int> nextNodes; // indices of possible next dialog nodes
    };
    
    void showDialog(const std::string& dialogId);
    void addDialogOption(const std::string& optionText, std::function<void()> callback);
    void update();
    void clearDialogs();

private:
    DialogSystem() = default;
    ~DialogSystem() = default;
    DialogSystem(const DialogSystem&) = delete;
    DialogSystem& operator=(const DialogSystem&) = delete;
    
    std::vector<DialogNode> dialogs;
};

} // namespace CmakeProject1
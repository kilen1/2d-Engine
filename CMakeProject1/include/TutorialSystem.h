#pragma once
#include <string>

namespace CmakeProject1 {

class TutorialSystem {
public:
    static TutorialSystem& getInstance();
    
    struct TutorialStep {
        std::string id;
        std::string instruction;
        std::string condition; // What needs to happen to advance
    };
    
    void startTutorial(const std::string& tutorialId);
    void addStep(const TutorialStep& step);
    void completeCurrentStep();
    void skipTutorial();
    bool isActive() const;
    void update();

private:
    TutorialSystem() = default;
    ~TutorialSystem() = default;
    TutorialSystem(const TutorialSystem&) = delete;
    TutorialSystem& operator=(const TutorialSystem&) = delete;
    
    std::string activeTutorial;
    int currentStep;
    bool tutorialActive;
    std::vector<TutorialStep> steps;
};

} // namespace CmakeProject1
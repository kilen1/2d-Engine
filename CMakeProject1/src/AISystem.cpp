#include "../include/AISystem.h"
#include <iostream>

namespace CmakeProject1 {

    // AIBrain implementations
    AIBrain::AIBrain(GameObject* owner) 
        : owner(owner), currentBehavior(AIBehaviorType::Idle), target(nullptr), 
          currentWaypointIndex(0) {
        // Initialize AI brain
    }

    AIBrain::~AIBrain() {
        // Clean up AI brain resources
    }

    void AIBrain::Update(float deltaTime) {
        // Update AI behavior based on current state
        if (!stateStack.empty()) {
            stateStack.back()->Update(this, deltaTime);
        } else {
            // Default behavior based on type
            switch (currentBehavior) {
                case AIBehaviorType::Patrol:
                    // Implement patrol logic
                    break;
                case AIBehaviorType::Chase:
                    // Implement chase logic
                    break;
                case AIBehaviorType::Attack:
                    // Implement attack logic
                    break;
                default:
                    break;
            }
        }
    }

    void AIBrain::SetBehavior(AIBehaviorType behavior) {
        currentBehavior = behavior;
    }

    AIBehaviorType AIBrain::GetBehavior() const {
        return currentBehavior;
    }

    void AIBrain::SetTarget(GameObject* target) {
        this->target = target;
    }

    GameObject* AIBrain::GetTarget() const {
        return target;
    }

    void AIBrain::ClearTarget() {
        target = nullptr;
    }

    std::vector<GameObject*> AIBrain::GetVisibleObjects() const {
        return visibleObjects;
    }

    bool AIBrain::CanSee(const GameObject* object) const {
        // Implement vision check
        return false;
    }

    bool AIBrain::CanHear(const GameObject* object) const {
        // Implement hearing check
        return false;
    }

    bool AIBrain::InAttackRange(const GameObject* object) const {
        // Implement attack range check
        return false;
    }

    void AIBrain::RequestPathTo(const Vector2& destination) {
        // Request pathfinding to destination
    }

    void AIBrain::RequestPathTo(const GameObject* target) {
        if (target) {
            RequestPathTo(target->GetPosition()); // Assuming GameObject has GetPosition method
        }
    }

    bool AIBrain::HasPath() const {
        return !path.empty() && currentWaypointIndex < path.size();
    }

    Vector2 AIBrain::GetNextWaypoint() {
        if (HasPath()) {
            return path[currentWaypointIndex];
        }
        return GetOwnerPosition();
    }

    void AIBrain::ClearPath() {
        path.clear();
        currentWaypointIndex = 0;
    }

    void AIBrain::PushState(std::unique_ptr<AIState> state) {
        if (!stateStack.empty()) {
            stateStack.back()->Exit(this);
        }
        stateStack.push_back(std::move(state));
        stateStack.back()->Enter(this);
    }

    void AIBrain::PopState() {
        if (!stateStack.empty()) {
            stateStack.back()->Exit(this);
            stateStack.pop_back();
            if (!stateStack.empty()) {
                stateStack.back()->Enter(this);
            }
        }
    }

    void AIBrain::ChangeState(std::unique_ptr<AIState> state) {
        if (!stateStack.empty()) {
            stateStack.back()->Exit(this);
            stateStack.pop_back();
        }
        stateStack.push_back(std::move(state));
        stateStack.back()->Enter(this);
    }

    std::string AIBrain::GetCurrentStateName() const {
        if (!stateStack.empty()) {
            return stateStack.back()->GetName();
        }
        return "No State";
    }

    Vector2 AIBrain::GetOwnerPosition() const {
        // Assuming GameObject has a method to get position
        return {0, 0}; // Placeholder
    }

    Vector2 AIBrain::GetDirectionTo(const Vector2& target) const {
        Vector2 ownerPos = GetOwnerPosition();
        return {target.x - ownerPos.x, target.y - ownerPos.y};
    }

    float AIBrain::GetDistanceTo(const Vector2& target) const {
        Vector2 dir = GetDirectionTo(target);
        return sqrt(dir.x * dir.x + dir.y * dir.y);
    }

    // AISystem implementations
    AISystem& AISystem::getInstance() {
        static AISystem instance;
        return instance;
    }

    AISystem::AISystem() : perceptionRange(10.0f), debugDrawPaths(false), maxActiveBrains(100) {
        // Initialize AI system
    }

    AISystem::~AISystem() {
        // Clean up AI system resources
        for (auto* brain : aiBrains) {
            delete brain;
        }
        aiBrains.clear();
    }

    bool AISystem::Initialize() {
        // Initialize AI system
        return true;
    }

    void AISystem::Shutdown() {
        // Shutdown AI system
        for (auto* brain : aiBrains) {
            delete brain;
        }
        aiBrains.clear();
        behaviorTrees.clear();
        groups.clear();
    }

    AIBrain* AISystem::CreateAIBrain(GameObject* owner) {
        AIBrain* brain = new AIBrain(owner);
        aiBrains.push_back(brain);
        return brain;
    }

    void AISystem::DestroyAIBrain(AIBrain* brain) {
        if (brain) {
            aiBrains.erase(
                std::remove(aiBrains.begin(), aiBrains.end(), brain),
                aiBrains.end()
            );
            delete brain;
        }
    }

    void AISystem::RegisterAIBrain(AIBrain* brain) {
        if (brain && std::find(aiBrains.begin(), aiBrains.end(), brain) == aiBrains.end()) {
            aiBrains.push_back(brain);
        }
    }

    void AISystem::UnregisterAIBrain(AIBrain* brain) {
        aiBrains.erase(
            std::remove(aiBrains.begin(), aiBrains.end(), brain),
            aiBrains.end()
        );
    }

    void AISystem::Update(float deltaTime) {
        for (auto* brain : aiBrains) {
            brain->Update(deltaTime);
        }
    }

    void AISystem::FixedUpdate(float fixedDeltaTime) {
        // Fixed update for physics-based AI
    }

    std::vector<Vector2> AISystem::FindPath(const Vector2& start, const Vector2& end, PathfindingAlgorithm algorithm) {
        // Implement pathfinding based on algorithm
        return {}; // Placeholder
    }

    void AISystem::SetPathfindingGrid(const std::vector<std::vector<int>>& grid) {
        pathfindingGrid = grid;
    }

    void AISystem::UpdatePathfindingGrid(int x, int y, int value) {
        if (x >= 0 && x < pathfindingGrid.size() && y >= 0 && y < pathfindingGrid[0].size()) {
            pathfindingGrid[x][y] = value;
        }
    }

    BehaviorTree* AISystem::CreateBehaviorTree(const std::string& name) {
        // Create a new behavior tree
        return nullptr; // Placeholder
    }

    void AISystem::DestroyBehaviorTree(const std::string& name) {
        behaviorTrees.erase(name);
    }

    BehaviorTree* AISystem::GetBehaviorTree(const std::string& name) {
        auto it = behaviorTrees.find(name);
        return it != behaviorTrees.end() ? it->second.get() : nullptr;
    }

    void AISystem::SetPerceptionRange(float range) {
        perceptionRange = range;
    }

    float AISystem::GetPerceptionRange() const {
        return perceptionRange;
    }

    std::vector<GameObject*> AISystem::FindObjectsInRadius(const Vector2& position, float radius) const {
        // Find game objects within radius
        return {}; // Placeholder
    }

    void AISystem::FormGroup(const std::string& groupName, const std::vector<AIBrain*>& brains) {
        groups[groupName] = brains;
    }

    void AISystem::DisbandGroup(const std::string& groupName) {
        groups.erase(groupName);
    }

    std::vector<AIBrain*> AISystem::GetGroupMembers(const std::string& groupName) const {
        auto it = groups.find(groupName);
        return it != groups.end() ? it->second : std::vector<AIBrain*>();
    }

    void AISystem::DebugDrawPaths(bool enable) {
        debugDrawPaths = enable;
    }

    void AISystem::SetMaxActiveBrains(int max) {
        maxActiveBrains = max;
    }

    int AISystem::GetActiveBrainCount() const {
        return aiBrains.size();
    }

}
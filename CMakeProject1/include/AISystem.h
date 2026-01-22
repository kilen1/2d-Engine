#pragma once

#include "GameObject.h"
#include "../Core/Types.h"
#include "AI/BehaviorTree.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace CmakeProject1 {

    // Forward declarations
    class AIBrain;

    // Enums for AI types
    enum class AIBehaviorType {
        Idle,
        Patrol,
        Chase,
        Flee,
        Attack,
        Defend,
        Wander,
        Follow,
        Evade,
        Pursue
    };

    enum class PathfindingAlgorithm {
        AStar,
        Dijkstra,
        BreadthFirst,
        DepthFirst,
        JumpPointSearch
    };

    // AI State Machine
    class AIState {
    public:
        virtual ~AIState() = default;
        virtual void Enter(AIBrain* brain) = 0;
        virtual void Update(AIBrain* brain, float deltaTime) = 0;
        virtual void Exit(AIBrain* brain) = 0;
        virtual std::string GetName() const = 0;
    };

    // Main AI Brain component
    class AIBrain {
    public:
        AIBrain(GameObject* owner);
        ~AIBrain();

        // Core AI functionality
        void Update(float deltaTime);
        void SetBehavior(AIBehaviorType behavior);
        AIBehaviorType GetBehavior() const;

        // Target management
        void SetTarget(GameObject* target);
        GameObject* GetTarget() const;
        void ClearTarget();

        // Perception system
        std::vector<GameObject*> GetVisibleObjects() const;
        bool CanSee(const GameObject* object) const;
        bool CanHear(const GameObject* object) const;
        bool InAttackRange(const GameObject* object) const;

        // Pathfinding
        void RequestPathTo(const Vector2& destination);
        void RequestPathTo(const GameObject* target);
        bool HasPath() const;
        Vector2 GetNextWaypoint();
        void ClearPath();

        // State machine
        void PushState(std::unique_ptr<AIState> state);
        void PopState();
        void ChangeState(std::unique_ptr<AIState> state);
        std::string GetCurrentStateName() const;

        // Utility functions
        Vector2 GetOwnerPosition() const;
        Vector2 GetDirectionTo(const Vector2& target) const;
        float GetDistanceTo(const Vector2& target) const;

    private:
        GameObject* owner;
        AIBehaviorType currentBehavior;
        GameObject* target;
        std::vector<Vector2> path;
        int currentWaypointIndex;
        std::vector<std::unique_ptr<AIState>> stateStack;
        std::vector<GameObject*> visibleObjects;
    };

    // AI System manager
    class AISystem {
    public:
        static AISystem& getInstance();

        // Initialization
        bool Initialize();
        void Shutdown();

        // AI management
        AIBrain* CreateAIBrain(GameObject* owner);
        void DestroyAIBrain(AIBrain* brain);
        void RegisterAIBrain(AIBrain* brain);
        void UnregisterAIBrain(AIBrain* brain);

        // Global AI updates
        void Update(float deltaTime);
        void FixedUpdate(float fixedDeltaTime);

        // Pathfinding
        std::vector<Vector2> FindPath(const Vector2& start, const Vector2& end, PathfindingAlgorithm algorithm = PathfindingAlgorithm::AStar);
        void SetPathfindingGrid(const std::vector<std::vector<int>>& grid);
        void UpdatePathfindingGrid(int x, int y, int value);

        // Behavior trees
        BehaviorTree* CreateBehaviorTree(const std::string& name);
        void DestroyBehaviorTree(const std::string& name);
        BehaviorTree* GetBehaviorTree(const std::string& name);

        // Perception system
        void SetPerceptionRange(float range);
        float GetPerceptionRange() const;
        std::vector<GameObject*> FindObjectsInRadius(const Vector2& position, float radius) const;

        // Group AI
        void FormGroup(const std::string& groupName, const std::vector<AIBrain*>& brains);
        void DisbandGroup(const std::string& groupName);
        std::vector<AIBrain*> GetGroupMembers(const std::string& groupName) const;

        // Utilities
        void DebugDrawPaths(bool enable);
        void SetMaxActiveBrains(int max);
        int GetActiveBrainCount() const;

    private:
        AISystem();
        ~AISystem();

        std::vector<AIBrain*> aiBrains;
        std::unordered_map<std::string, std::unique_ptr<BehaviorTree>> behaviorTrees;
        std::unordered_map<std::string, std::vector<AIBrain*>> groups;
        std::vector<std::vector<int>> pathfindingGrid;
        float perceptionRange = 10.0f;
        bool debugDrawPaths = false;
        int maxActiveBrains = 100;
    };

} // namespace CmakeProject1
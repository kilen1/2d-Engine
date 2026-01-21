#pragma once

#include <string>

namespace CmakeProject1 {
    class BehaviorTreeManager {
    public:
        virtual ~BehaviorTreeManager() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void loadBehaviorTree(const std::string& name, const std::string& path) = 0;
        virtual void executeBehaviorTree(const std::string& name) = 0;
    };
}
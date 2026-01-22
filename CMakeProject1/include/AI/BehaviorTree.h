#pragma once

#include <string>
#include <memory>

namespace CmakeProject1 {
    
    // Base class for behavior tree nodes
    class BehaviorTreeNode {
    public:
        enum class Status {
            Success,
            Failure,
            Running
        };

        virtual ~BehaviorTreeNode() = default;
        virtual Status execute() = 0;
        virtual void reset() {}
    };

    // The BehaviorTree class that was previously forward-declared
    class BehaviorTree {
    public:
        BehaviorTree() = default;
        explicit BehaviorTree(const std::string& name);
        virtual ~BehaviorTree() = default;

        void setName(const std::string& name);
        const std::string& getName() const;

        virtual bool initialize();
        virtual void update();
        virtual void shutdown();

        virtual void setRoot(std::shared_ptr<BehaviorTreeNode> root);
        virtual std::shared_ptr<BehaviorTreeNode> getRoot() const;
        virtual BehaviorTreeNode::Status run();

    protected:
        std::string name_;
        std::shared_ptr<BehaviorTreeNode> root_;
    };

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
#include "../../include/AI/BehaviorTree.h"

namespace CmakeProject1 {

    BehaviorTree::BehaviorTree(const std::string& name) : name_(name), root_(nullptr) {
    }

    void BehaviorTree::setName(const std::string& name) {
        name_ = name;
    }

    const std::string& BehaviorTree::getName() const {
        return name_;
    }

    bool BehaviorTree::initialize() {
        return true;
    }

    void BehaviorTree::update() {
        // Update the behavior tree if needed
    }

    void BehaviorTree::shutdown() {
        // Clean up resources
    }

    void BehaviorTree::setRoot(std::shared_ptr<BehaviorTreeNode> root) {
        root_ = root;
    }

    std::shared_ptr<BehaviorTreeNode> BehaviorTree::getRoot() const {
        return root_;
    }

    BehaviorTreeNode::Status BehaviorTree::run() {
        if (root_) {
            return root_->execute();
        }
        return BehaviorTreeNode::Status::Failure;
    }

}
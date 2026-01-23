#pragma once
#include "Types.h"
#include <string>
#include <memory>
#include <functional>

namespace CmakeProject1 {

    class GameObject;

    class Script {
    public:
        Script() = default;
        virtual ~Script() = default;

        std::string name;
        bool enabled = true;
        std::weak_ptr<GameObject> owner;

        // Lifecycle
        virtual void onStart() {}
        virtual void onUpdate(float deltaTime) {}
        virtual void onFixedUpdate(float fixedDeltaTime) {}
        virtual void onDestroy() {}

        // Optional: custom update logic
        std::function<void(float)> customUpdate;
    };

} // namespace CmakeProject1
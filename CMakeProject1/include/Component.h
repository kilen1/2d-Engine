#pragma once

#include <memory>
#include <SDL2/SDL.h>

namespace CmakeProject1 {

    class GameObject;

    class Component {
    public:
        Component();
        virtual ~Component();

        virtual void update(float deltaTime) {}
        virtual void render(SDL_Renderer* renderer) {}

        void setOwner(std::shared_ptr<GameObject> newOwner) { owner = newOwner; }
        std::shared_ptr<GameObject> getOwner() const { return owner.lock(); }

        bool isEnabled() const { return enabled; }
        void setEnabled(bool state) { enabled = state; }

    protected:
        std::weak_ptr<GameObject> owner;
        bool enabled = true;
    };

} // namespace CmakeProject1
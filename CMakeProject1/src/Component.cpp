#include "Component.h"
#include "GameObject.h"
#include "Scene.h"

namespace CmakeProject1 {

    std::shared_ptr<Transform> Component::getTransform() const {
        auto gameObject = m_gameObject.lock();
        if (gameObject) {
            return gameObject->getTransform();
        }
        return nullptr;
    }

    std::shared_ptr<Scene> Component::getScene() const {
        auto gameObject = m_gameObject.lock();
        if (gameObject) {
            return gameObject->getScene();
        }
        return nullptr;
    }

    Engine* Component::getEngine() const {
        // Scene не предоставляет getEngine(), поэтому возвращаем nullptr
        // Если у вас есть глобальная переменная или Singleton для Engine, используйте её
        // Например: return g_EngineInstance;
        return nullptr; // или замените на ваш способ получения Engine
    }

}
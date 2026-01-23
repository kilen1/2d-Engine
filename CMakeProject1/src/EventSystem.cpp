#include "EventSystem.h"
#include <iostream>


namespace CmakeProject1 {

    EventSystem::EventSystem() {}

    EventSystem::~EventSystem() {
        shutdown();
    }

    void EventSystem::initialize() {
        std::cout << "EventSystem initialized." << std::endl;
    }

    void EventSystem::shutdown() {
        clearAllSubscriptions();
    }

    size_t EventSystem::subscribe(const std::string& eventType, std::function<void(const Event&)> callback) {
        size_t id = m_nextId++;
        m_subscribers[eventType].push_back({ id, callback });
        return id;
    }

    void EventSystem::unsubscribe(const std::string& eventType, size_t id) {
        auto& list = m_subscribers[eventType];
        list.erase(
            std::remove_if(list.begin(), list.end(),
                [id](const auto& pair) { return pair.first == id; }),
            list.end()
        );
        if (list.empty()) {
            m_subscribers.erase(eventType);
        }
    }

    void EventSystem::emit(const std::string& eventType, const Event& event) {
        auto it = m_subscribers.find(eventType);
        if (it != m_subscribers.end()) {
            for (const auto& [id, callback] : it->second) {
                callback(event);
            }
        }
    }

    void EventSystem::clearAllSubscriptions() {
        m_subscribers.clear();
        m_nextId = 0;
    }

} // namespace CmakeProject1
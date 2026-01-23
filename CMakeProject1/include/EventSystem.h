#pragma once
#include "Types.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <atomic>

namespace CmakeProject1 {

    

    class EventSystem {
    public:
        EventSystem();
        ~EventSystem();

        void initialize();
        void shutdown();

        // Subscribe to event
        size_t subscribe(const std::string& eventType, std::function<void(const Event&)> callback);
        void unsubscribe(const std::string& eventType, size_t id);

        // Emit event
        void emit(const std::string& eventType, const Event& event);

        // Utility
        void clearAllSubscriptions();

    private:
        std::atomic<size_t> m_nextId{ 0 };
        std::unordered_map<std::string, std::vector<std::pair<size_t, std::function<void(const Event&)>>>> m_subscribers;
    };

} // namespace CmakeProject1
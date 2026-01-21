#pragma once

#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <any>

namespace CmakeProject1 {
    struct Event {
        std::string type;
        void* data = nullptr;
    };

    using EventData = std::unordered_map<std::string, std::any>;

    class EventSystem {
    public:
        virtual ~EventSystem() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void subscribe(const std::string& eventType, std::function<void(const Event&)> callback) = 0;
        virtual void unsubscribe(const std::string& eventType, size_t listenerId) = 0;
        virtual void emit(const std::string& eventType, const EventData& data = {}) = 0;
    };
}
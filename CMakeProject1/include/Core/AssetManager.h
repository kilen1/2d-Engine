#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace CmakeProject1 {
    class Resource {
    public:
        virtual ~Resource() = default;
        std::string name;
        size_t size = 0;
    };

    class AssetManager {
    public:
        virtual ~AssetManager() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        template<typename T>
        std::shared_ptr<T> load(const std::string& name) {
            // Implementation would go here
            return nullptr;
        }

        template<typename T>
        void unload(const std::string& name) {
            // Implementation would go here
        }
    };
}
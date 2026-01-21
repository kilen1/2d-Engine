#pragma once

#include <string>

namespace CmakeProject1 {
    class Profiler {
    public:
        virtual ~Profiler() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void startProfiling(const std::string& sessionName) = 0;
        virtual void stopProfiling() = 0;
        virtual void saveProfileData(const std::string& path) = 0;
        virtual void show() = 0;
        virtual void hide() = 0;
    };
}
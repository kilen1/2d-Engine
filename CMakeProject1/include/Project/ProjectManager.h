#pragma once

#include <string>
#include <filesystem>

namespace CmakeProject1 {
    class ProjectManager {
    public:
        virtual ~ProjectManager() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual bool createProject(const std::string& name, const std::filesystem::path& location) = 0;
        virtual bool loadProject(const std::filesystem::path& projectPath) = 0;
        virtual bool saveProject() = 0;
        virtual void closeProject() = 0;
    };
}
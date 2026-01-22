#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Core/Types.h"

namespace CmakeProject1 {

    class EditorTool {
    public:
        virtual ~EditorTool() = default;
        virtual void activate() {}
        virtual void deactivate() {}
        virtual void update(float deltaTime) {}
        virtual void render() {}
    };

    class NodeGraph {
    public:
        virtual ~NodeGraph() = default;
    };

    class ShaderEditor {
    public:
        virtual ~ShaderEditor() = default;
    };

    class MaterialEditor {
    public:
        virtual ~MaterialEditor() = default;
    };

    class TextureEditor {
    public:
        virtual ~TextureEditor() = default;
    };

    class SpriteEditor {
    public:
        virtual ~SpriteEditor() = default;
    };

    class AnimationEditor {
    public:
        virtual ~AnimationEditor() = default;
    };

    class ParticleEditor {
    public:
        virtual ~ParticleEditor() = default;
    };

    class TerrainEditor {
    public:
        virtual ~TerrainEditor() = default;
    };

    class WaterEditor {
    public:
        virtual ~WaterEditor() = default;
    };

    class LightEditor {
    public:
        virtual ~LightEditor() = default;
    };

    class PostProcessingEditor {
    public:
        virtual ~PostProcessingEditor() = default;
    };

    class PhysicsEditor {
    public:
        virtual ~PhysicsEditor() = default;
    };

    class NavigationEditor {
    public:
        virtual ~NavigationEditor() = default;
    };

    class UIEditor {
    public:
        virtual ~UIEditor() = default;
    };

    class ScriptEditor {
    public:
        virtual ~ScriptEditor() = default;
    };

    class BuildSettings {
    public:
        std::string platform = "Windows";
        std::string configuration = "Debug";
        bool developmentBuild = true;
        bool headless = false;
    };

    struct PackageInfo {
        std::string name;
        std::string version;
        std::string description;
    };

    class VersionControl {
    public:
        virtual ~VersionControl() = default;
    };

    class Profiler {
    public:
        virtual ~Profiler() = default;
    };

    class NetworkEditor {
    public:
        virtual ~NetworkEditor() = default;
    };

    struct EditorSettings {
        std::string theme = "Dark";
        std::string language = "English";
        bool autoSave = true;
        int autoSaveInterval = 5; // minutes
    };

    struct Macro {
        std::string name;
        std::vector<std::string> actions;
    };

    class Plugin {
    public:
        virtual ~Plugin() = default;
    };

    struct Tutorial {
        std::string title;
        std::string description;
        std::vector<std::string> steps;
    };

    struct Message {
        MessageType type;
        std::string title;
        std::string content;
        float timestamp;
    };

    class Dialog {
    public:
        virtual ~Dialog() = default;
    };

    class PreviewWindow {
    public:
        virtual ~PreviewWindow() = default;
    };

    class QuickSearch {
    public:
        virtual ~QuickSearch() = default;
    };

    struct ProjectHistory {
        std::string projectName;
        std::string projectPath;
        std::string lastOpened;
    };

    struct Backup {
        std::string id;
        std::string date;
        std::string description;
        std::string path;
    };

    struct EditorStatistics {
        int objectsCount = 0;
        int componentsCount = 0;
        int texturesCount = 0;
        int materialsCount = 0;
        float memoryUsed = 0.0f;
    };

    struct DebugInfo {
        bool showWireframe = false;
        bool showColliders = false;
        bool showBounds = false;
        bool showNormals = false;
    };
}
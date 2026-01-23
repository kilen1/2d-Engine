#include "Engine.h"
#include "Editor.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <json/json.h>



namespace CmakeProject1 {

    bool Engine::createProject(const std::string& name, const std::string& path) {
        SPDLOG_INFO("Creating project: {} at {}", name, path);

        if (name.empty()) {
            SPDLOG_ERROR("Project name cannot be empty");
            return false;
        }

        std::filesystem::path projectPath = std::filesystem::path(path) / name;

        if (std::filesystem::exists(projectPath)) {
            SPDLOG_ERROR("Project directory already exists: {}", projectPath.string());
            return false;
        }

        // Create project directory
        if (!std::filesystem::create_directories(projectPath)) {
            SPDLOG_ERROR("Failed to create project directory: {}", projectPath.string());
            return false;
        }

        // Create project structure
        createDefaultProjectStructure(projectPath, name);

        // Create project file
        std::string projectFile = generateProjectFile(name);
        std::ofstream file(projectPath / (name + ".project"));
        if (!file.is_open()) {
            SPDLOG_ERROR("Failed to create project file");
            return false;
        }

        file << projectFile;
        file.close();

        // Add to recent projects
        m_config.recentProjects.push_back((projectPath / (name + ".project")).string());
        if (m_config.recentProjects.size() > 10) {
            m_config.recentProjects.erase(m_config.recentProjects.begin());
        }

        // Load the new project
        if (!openProject((projectPath / (name + ".project")).string())) {
            SPDLOG_ERROR("Failed to load newly created project");
            return false;
        }

        SPDLOG_INFO("Project created successfully: {}", name);
        return true;
    }

    void Engine::createDefaultProjectStructure(const std::filesystem::path& projectPath, const std::string& projectName) const {
        // Create directories
        std::vector<std::string> directories = {
            "Assets",
            "Assets/Sprites",
            "Assets/Textures",
            "Assets/Audio",
            "Assets/Music",
            "Assets/Fonts",
            "Assets/Materials",
            "Assets/Shaders",
            "Assets/Prefabs",
            "Assets/Animations",
            "Assets/UI",
            "Assets/Tilemaps",
            "Assets/Terrains",
            "Scripts",
            "Scenes",
            "Settings",
            "Build",
            "Build/Windows",
            "Build/Linux",
            "Build/macOS",
            "Build/Web",
            "Build/Android",
            "Build/iOS",
            "Docs",
            "Tests"
        };

        for (const auto& dir : directories) {
            std::filesystem::create_directories(projectPath / dir);
        }

        // Create default files

        // Default scene
        Json::Value scene;
        scene["name"] = "MainScene";
        scene["settings"]["backgroundColor"] = Json::Value(Json::arrayValue);
        scene["settings"]["backgroundColor"].append(0.1);
        scene["settings"]["backgroundColor"].append(0.1);
        scene["settings"]["backgroundColor"].append(0.1);
        scene["settings"]["backgroundColor"].append(1.0);
        scene["settings"]["gravity"] = Json::Value(Json::arrayValue);
        scene["settings"]["gravity"].append(0);
        scene["settings"]["gravity"].append(-9.81);

        Json::StreamWriterBuilder writer;
        writer["indentation"] = "    ";
        std::string sceneJson = Json::writeString(writer, scene);

        std::ofstream sceneFile(projectPath / "Scenes" / "MainScene.scene");
        sceneFile << sceneJson;
        sceneFile.close();

        // Default player script
        std::ofstream scriptFile(projectPath / "Scripts" / "Player.lua");
        scriptFile << R"(--- 
-- Player.lua
-- Default player controller
--

local Player = {}

-- Public properties (visible in inspector)
Player.speed = 5.0
Player.jumpForce = 10.0
Player.maxHealth = 100
Player.currentHealth = 100

-- Private variables
local isGrounded = false
local transform = nil
local sprite = nil
local rigidbody = nil

function Player:Start()
    -- Get components
    transform = self:GetComponent("Transform")
    sprite = self:GetComponent("SpriteRenderer")
    rigidbody = self:GetComponent("Rigidbody2D")
    
    -- Initialize health
    self.currentHealth = self.maxHealth
    
    Log:Info("Player script started")
end

function Player:Update(deltaTime)
    -- Movement
    local horizontal = Input:GetAxis("Horizontal")
    local vertical = Input:GetAxis("Vertical")
    
    local velocity = Vector2(horizontal * self.speed, 0)
    
    if rigidbody then
        rigidbody.velocity = velocity
    else
        transform:Translate(velocity * deltaTime)
    end
    
    -- Jump
    if Input:GetKeyDown("Space") and isGrounded then
        if rigidbody then
            rigidbody:AddForce(Vector2(0, self.jumpForce), ForceMode2D.Impulse)
        end
        isGrounded = false
    end
    
    -- Flip sprite based on direction
    if math.abs(horizontal) > 0.1 then
        sprite.flipX = horizontal < 0
    end
    
    -- Debug controls
    if Input:GetKeyDown("F1") then
        Log:Debug("Player position: " .. tostring(transform.position))
    end
end

function Player:FixedUpdate(deltaTime)
    -- Fixed update for physics
end

function Player:OnCollisionEnter(collision)
    if collision.other.tag == "Ground" then
        isGrounded = true
    end
    
    if collision.other.tag == "Enemy" then
        self:TakeDamage(10)
    end
end

function Player:OnCollisionExit(collision)
    if collision.other.tag == "Ground" then
        isGrounded = false
    end
end

function Player:TakeDamage(amount)
    self.currentHealth = self.currentHealth - amount
    
    Log:Warning("Player took damage: " .. amount .. " | Health: " .. self.currentHealth)
    
    if self.currentHealth <= 0 then
        self:Die()
    end
end

function Player:Die()
    Log:Error("Player died!")
    
    -- Destroy player after 2 seconds
    Timer:Delay(2.0, function()
        self.gameObject:Destroy()
    end)
end

function Player:OnDestroy()
    Log:Info("Player destroyed")
end

return Player
)";
        scriptFile.close();

        // Default material
        std::ofstream materialFile(projectPath / "Assets" / "Materials" / "Default.material");
        materialFile << R"({
    "name": "Default",
    "shader": "Assets/Shaders/Default.shader",
    "properties": {
        "color": [1.0, 1.0, 1.0, 1.0],
        "mainTexture": null,
        "tiling": [1.0, 1.0],
        "offset": [0.0, 0.0]
    }
})";
        materialFile.close();

        // Default shader
        std::filesystem::create_directories(projectPath / "Assets" / "Shaders");

        std::ofstream vertexShader(projectPath / "Assets" / "Shaders" / "Default.vert");
        vertexShader << R"(#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    vTexCoord = aTexCoord;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 0.0, 1.0);
}
)";
        vertexShader.close();

        std::ofstream fragmentShader(projectPath / "Assets" / "Shaders" / "Default.frag");
        fragmentShader << R"(#version 330 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform vec4 uColor;
uniform sampler2D uTexture;
uniform vec2 uTiling;
uniform vec2 uOffset;

void main() {
    vec2 texCoord = vTexCoord * uTiling + uOffset;
    vec4 texColor = texture(uTexture, texCoord);
    FragColor = texColor * uColor;
}
)";
        fragmentShader.close();

        // Create shader file reference
        Json::Value shader;
        shader["name"] = "Default";
        shader["vertex"] = "Assets/Shaders/Default.vert";
        shader["fragment"] = "Assets/Shaders/Default.frag";

        std::string shaderJson = Json::writeString(writer, shader);
        std::ofstream shaderFile(projectPath / "Assets" / "Shaders" / "Default.shader");
        shaderFile << shaderJson;
        shaderFile.close();

        // Create README
        std::ofstream readmeFile(projectPath / "README.md");
        readmeFile << R"(# )" << projectName << R"(

## Project Description

This is a 2D game project created with the 2D Game Engine.

## Project Structure

- `Assets/` - All game assets (sprites, sounds, fonts, etc.)
- `Scripts/` - Lua scripts for game logic
- `Scenes/` - Game scenes
- `Settings/` - Project settings and configuration
- `Build/` - Build outputs for different platforms
- `Docs/` - Project documentation
- `Tests/` - Unit tests

## Getting Started

1. Open this project in the 2D Game Engine
2. Edit the `MainScene` in the Scene View
3. Modify the `Player.lua` script for your game logic
4. Press Play to test your game
5. Build your project for your target platform

## Scripting API

See the Scripting Reference in the engine's Help menu for the complete API.

## License

Add your license information here.

## Credits

Add your credits here.
)";
        readmeFile.close();
    }

    std::string Engine::generateProjectFile(const std::string& name){
        Json::Value project;

        project["name"] = name;
        project["version"] = "1.0.0";
        project["engineVersion"] = getVersion();
        project["created"] = static_cast<Json::UInt64>(std::time(nullptr));
        project["modified"] = static_cast<Json::UInt64>(std::time(nullptr));

        project["startScene"] = "Scenes/MainScene.scene";

        // Settings
        project["settings"]["window"]["width"] = 1280;
        project["settings"]["window"]["height"] = 720;
        project["settings"]["window"]["fullscreen"] = false;
        project["settings"]["window"]["vsync"] = true;
        project["settings"]["window"]["resizable"] = true;
        project["settings"]["window"]["title"] = name;

        project["settings"]["graphics"]["targetFPS"] = 60;
        project["settings"]["graphics"]["antialiasing"] = true;
        project["settings"]["graphics"]["lighting"] = true;
        project["settings"]["graphics"]["shadows"] = false;
        project["settings"]["graphics"]["bloom"] = false;

        project["settings"]["audio"]["masterVolume"] = 1.0;
        project["settings"]["audio"]["musicVolume"] = 0.8;
        project["settings"]["audio"]["sfxVolume"] = 0.9;
        project["settings"]["audio"]["enabled"] = true;

        project["settings"]["physics"]["gravity"] = Json::Value(Json::arrayValue);
        project["settings"]["physics"]["gravity"].append(0);
        project["settings"]["physics"]["gravity"].append(-9.81);
        project["settings"]["physics"]["enabled"] = true;
        project["settings"]["physics"]["fixedTimestep"] = 0.0166667;

        project["settings"]["input"]["mouseSensitivity"] = 1.0;
        project["settings"]["input"]["gamepadDeadzone"] = 0.2;
        project["settings"]["input"]["tabletEnabled"] = true;

        project["settings"]["scripting"]["language"] = "Lua";
        project["settings"]["scripting"]["hotReload"] = true;
        project["settings"]["scripting"]["debug"] = false;

        // Build settings
        project["build"]["target"] = "Windows";
        project["build"]["configuration"] = "Development";
        project["build"]["debug"] = false;
        project["build"]["optimize"] = true;

        // Dependencies
        project["dependencies"] = Json::Value(Json::objectValue);

        Json::StreamWriterBuilder writer;
        writer["indentation"] = "    ";

        return Json::writeString(writer, project);
    }

} // namespace CmakeProject1
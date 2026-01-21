#include "../include/Engine.h"
#include "../include/GameObject.h"
#include "../include/SpriteRenderer.h"
#include "../include/Transform.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto& engine = CmakeProject1::Engine::getInstance();
        
        if (!engine.initialize(argc, argv)) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return -1;
        }
        
        // Create a simple scene with a sprite
        auto scene = std::make_shared<CmakeProject1::Scene>("Main Scene");
        
        // Create a game object with sprite renderer
        auto gameObject = scene->createGameObject("Player");
        auto spriteRenderer = gameObject->addComponent<CmakeProject1::SpriteRenderer>();
        
        // For now, just run the engine
        engine.run();
        engine.shutdown();
        
    } catch (const std::exception& e) {
        std::cerr << "Engine crashed with error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
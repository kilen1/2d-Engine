#include "Engine.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "Scene/SceneManager.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto& engine = CmakeProject1::Engine::getInstance();
        
        if (!engine.initialize(argc, argv)) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return -1;
        }
        
        // For now, just run the engine
        engine.run();
        engine.shutdown();
        
    } catch (const std::exception& e) {
        std::cerr << "Engine crashed with error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
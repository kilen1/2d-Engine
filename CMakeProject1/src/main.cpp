#include "Engine.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto& engine = CmakeProject1::Engine::getInstance();

        if (!engine.initialize(argc, argv)) {
            std::cerr << "" << std::endl;
            return -1;
        }

        engine.run();
        engine.shutdown();

    }
    catch (const std::exception& e) {
        std::cerr << " " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
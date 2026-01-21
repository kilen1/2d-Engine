#pragma once

#include <string>

namespace CmakeProject1 {
    class GameView {
    public:
        virtual ~GameView() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void render() = 0;
        virtual void setViewport(int x, int y, int w, int h) = 0;
    };
}
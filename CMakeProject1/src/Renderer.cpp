#include "Renderer.h"
#include <iostream>
#include <SDL2/SDL.h>
namespace CmakeProject1 {

    Renderer::Renderer() {}

    Renderer::~Renderer() {}

    bool Renderer::initialize() {
        // Инициализация OpenGL / Vulkan / DirectX
        std::cout << "Renderer initialized." << std::endl;
        return true;
    }

    void Renderer::shutdown() {
        std::cout << "Renderer shutdown." << std::endl;
    }

    

    void Renderer::clear(const Color& color) {
        // glClearColor(color.r, color.g, color.b, color.a);
        // glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::beginFrame() {
        clear(Color{ 0.1f, 0.1f, 0.1f, 1.0f });
    }

    void Renderer::endFrame() {
        // Swap buffers handled by Window
    }

    void Renderer::drawSprite(std::shared_ptr<Texture> texture, const Rect& src, const Rect& dest, float rotation) {
        // Реализация рисования спрайта
    }

    void Renderer::drawLine(const Vector2& start, const Vector2& end, const Color& color) {
        // Рисование линии
    }

    void Renderer::drawCircle(const Vector2& center, float radius, const Color& color) {
        // Рисование круга
    }

    void Renderer::setViewport(int x, int y, int w, int h) {
        // glViewport(x, y, w, h);
    }
    void CmakeProject1::Renderer::drawRect(const Rect& rect, const Color& color) {
        // TODO: implement drawRect
    }
    void Renderer::resetViewport() {
        // Сброс viewport на размер окна
    }

} // namespace CmakeProject1
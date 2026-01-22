#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>  // For OpenGL types like GLuint
#include <vector>
#include <string>
#include <functional>
#include "Core/Types.h"

namespace CmakeProject1 {

    enum class BlendMode {
        Normal,
        Additive,
        Multiply,
        Screen,
        Overlay
    };

    class GraphicsTablet {
    public:
        struct TabletData {
            float x = 0.0f;          // Нормализованная координата X (0-1)
            float y = 0.0f;          // Нормализованная координата Y (0-1)
            float pressure = 0.0f;   // Давление (0-1)
            float tiltX = 0.0f;      // Наклон по X (-1 to 1)
            float tiltY = 0.0f;      // Наклон по Y (-1 to 1)
            float rotation = 0.0f;   // Вращение пера (0-360)
            bool eraser = false;     // Ластик активен
            bool barrelButton = false; // Боковая кнопка
            int penButtons = 0;      // Состояние кнопок пера
            
            // Экранные координаты
            int screenX = 0;
            int screenY = 0;
        };
        
        GraphicsTablet();
        ~GraphicsTablet();
        
        bool initialize();
        void shutdown();
        
        void update();
        void handleEvent(const SDL_Event& event);
        
        bool isConnected() const { return connected; }
        bool isActive() const { return active; }
        
        const TabletData& getData() const { return currentData; }
        
        // События планшета
        using PressureCallback = std::function<void(float)>;
        using PositionCallback = std::function<void(float, float)>;
        using ButtonCallback = std::function<void(int, bool)>;
        
        void setPressureCallback(PressureCallback callback);
        void setPositionCallback(PositionCallback callback);
        void setButtonCallback(ButtonCallback callback);
        
        // Режимы рисования
        enum class BrushType {
            Pencil,
            Brush,
            Airbrush,
            Marker,
            Watercolor,
            Eraser
        };
        
        struct BrushSettings {
            BrushType type = BrushType::Pencil;
            float size = 10.0f;
            float hardness = 0.8f;
            float opacity = 1.0f;
            float flow = 1.0f;
            float spacing = 0.25f;
            Color color = Color::White;
            bool pressureSize = true;
            bool pressureOpacity = true;
            bool pressureFlow = true;
            bool smoothing = true;
            float smoothness = 0.5f;
        };
        
        BrushSettings& getBrushSettings() { return brushSettings; }
        
        // Слои рисования
        struct Layer {
            std::string name;
            GLuint textureId = 0;  // Renamed from 'texture' to avoid potential conflicts
            float opacity = 1.0f;
            bool visible = true;
            BlendMode blendMode = BlendMode::Normal;
        };
        
        void createLayer(const std::string& name);
        void removeLayer(int index);
        void setActiveLayer(int index);
        Layer* getActiveLayer();
        
        // Инструменты
        void beginStroke();
        void endStroke();
        void drawPoint(float x, float y);
        void drawLine(float x1, float y1, float x2, float y2);
        void drawBezier(float x1, float y1, float x2, float y2, 
                       float cx1, float cy1, float cx2, float cy2);
        
        // Отмена/повтор
        void undo();
        void redo();
        bool canUndo() const;
        bool canRedo() const;
        
        // Сохранение/загрузка
        bool saveDrawing(const std::string& path);
        bool loadDrawing(const std::string& path);
        
        // Экспорт
        bool exportToPNG(const std::string& path);
        bool exportToSVG(const std::string& path);
        bool exportToPSD(const std::string& path);
        
        // Фильтры
        void applyGaussianBlur(float radius);
        void applySharpen(float amount);
        void adjustBrightness(float value);
        void adjustContrast(float value);
        void adjustSaturation(float value);
        void adjustHue(float value);
        
        // Трансформации
        void rotate(float angle);
        void scale(float factor);
        void translate(float dx, float dy);
        void flipHorizontal();
        void flipVertical();
        
        // Выделение
        void beginSelection(float x, float y);
        void updateSelection(float x, float y);
        void endSelection();
        void clearSelection();
        void moveSelection(float dx, float dy);
        void rotateSelection(float angle);
        void scaleSelection(float factor);
        
        // Кисти
        struct BrushPreset {
            std::string name;
            BrushSettings settings;
            GLuint texture = 0;
        };
        
        void createBrushPreset(const std::string& name, const BrushSettings& settings);
        void loadBrushPreset(const std::string& name);
        void saveBrushLibrary(const std::string& path);
        void loadBrushLibrary(const std::string& path);
        
        // Кисти по умолчанию
        void createDefaultBrushes();
        
        // Текстуры кистей
        void generateBrushTexture(BrushSettings& settings);
        
        // Симметрия
        void enableSymmetry(bool enable);
        void setSymmetryAxes(int axes);
        void setSymmetryAngle(float angle);
        
        // Гид
        void enableRuler(bool enable);
        void setRulerPoints(const std::vector<Vector2>& points);
        
        // Перспектива
        void enablePerspectiveGuide(bool enable);
        void setPerspectivePoints(const std::vector<Vector2>& points);
        
    private:
        bool connected = false;
        bool active = false;
        TabletData currentData;
        TabletData previousData;
        
        SDL_Haptic* haptic = nullptr;
        
        BrushSettings brushSettings;
        std::vector<BrushPreset> brushPresets;
        
        std::vector<Layer> layers;
        int activeLayerIndex = -1;
        
        std::vector<TabletData> strokePoints;
        bool isDrawing = false;
        
        std::vector<std::vector<TabletData>> undoStack;
        std::vector<std::vector<TabletData>> redoStack;
        
        // Симметрия
        bool symmetryEnabled = false;
        int symmetryAxes = 1;
        float symmetryAngle = 0.0f;
        
        // Гид
        bool rulerEnabled = false;
        std::vector<Vector2> rulerPoints;
        
        // Перспектива
        bool perspectiveEnabled = false;
        std::vector<Vector2> perspectivePoints;
        
        // Коллбэки
        PressureCallback pressureCallback;
        PositionCallback positionCallback;
        ButtonCallback buttonCallback;
        
        // Шейдеры для рисования
        GLuint drawShader = 0;
        GLuint brushTexture = 0;
        GLuint canvasFramebuffer = 0;
        GLuint canvasTexture = 0;
        
        void initShaders();
        void initTextures();
        void setupFramebuffer(int width, int height);
        
        void processPressure(float pressure);
        void processPosition(float x, float y);
        void processButtons(int buttons);
        
        void interpolateStroke();
        void applyBrushToPoint(const TabletData& point);
        
        void saveState();
        void restoreState(const std::vector<TabletData>& state);
        
        Vector2 calculateSymmetryPoint(const Vector2& point, int index);
        std::vector<Vector2> calculatePerspectivePoints(const Vector2& point);
        
        void renderBrushPreview();
        void renderCanvas();
        
        // Вспомогательные функции для OpenGL
        GLuint compileShader(GLenum type, const char* source);
        GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
        void checkGLError(const char* context);
    };
}
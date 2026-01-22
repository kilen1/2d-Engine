#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace CmakeProject1 {

    // Basic types
    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    // Math types
    struct Vector2 {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
        Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

        float length() const { return sqrtf(x * x + y * y); }
        Vector2 normalized() const { float len = length(); return len > 0 ? Vector2(x / len, y / len) : Vector2(); }
        float dot(const Vector2& other) const { return x * other.x + y * other.y; }
    };

    struct Vector3 {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    };

    struct Vector4 {
        float x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(1) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    };

    struct Color {
        float r, g, b, a;

        Color() : r(1), g(1), b(1), a(1) {}
        Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

        static Color Red() { return Color(1, 0, 0); }
        static Color Green() { return Color(0, 1, 0); }
        static Color Blue() { return Color(0, 0, 1); }
        static Color White() { return Color(1, 1, 1); }
        static Color Black() { return Color(0, 0, 0); }
    };

    struct Rect {
        float x, y, width, height;

        Rect() : x(0), y(0), width(0), height(0) {}
        Rect(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}

        bool contains(const Vector2& point) const {
            return point.x >= x && point.x <= x + width &&
                point.y >= y && point.y <= y + height;
        }
    };

    // Matrix 4x4
    struct Matrix4 {
        float m[16];

        Matrix4() {
            identity();
        }

        void identity() {
            memset(m, 0, sizeof(m));
            m[0] = m[5] = m[10] = m[15] = 1.0f;
        }

        static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far) {
            Matrix4 result;
            result.m[0] = 2.0f / (right - left);
            result.m[5] = 2.0f / (top - bottom);
            result.m[10] = -2.0f / (far - near);
            result.m[12] = -(right + left) / (right - left);
            result.m[13] = -(top + bottom) / (top - bottom);
            result.m[14] = -(far + near) / (far - near);
            result.m[15] = 1.0f;
            return result;
        }

        static Matrix4 translate(const Vector2& translation) {
            Matrix4 result;
            result.identity();
            result.m[12] = translation.x;
            result.m[13] = translation.y;
            return result;
        }

        static Matrix4 scale(const Vector2& scaling) {
            Matrix4 result;
            result.identity();
            result.m[0] = scaling.x;
            result.m[5] = scaling.y;
            return result;
        }
    };

    // Event system
    struct Event {
        enum Type {
            WindowResized,
            KeyPressed,
            KeyReleased,
            MouseMoved,
            MousePressed,
            MouseReleased,
            MouseWheel,
            FileDropped,
            SceneChanged,
            GameObjectSelected,
            ProjectLoaded,
            PlayModeChanged
        };

        Type type;
        void* data;
        size_t dataSize;
    };

    // Forward declarations
    class GameObject;
    class Component;
    class Scene;
    class Asset;
    class Texture;
    class Sprite;
    class Material;
    class Shader;
    class Script;
    class Camera;
    class Light2D;
    class ParticleSystem;
    class UIElement;
    class Canvas;
    class BehaviorTree;
    class AudioClip;
    class Font;
    class AnimationClip;
    class Terrain2D;
    class Tilemap;
    class Rigidbody2D;
    class Collider2D;
    class Joint2D;

    // Component types
    enum class ComponentType {
        Transform = 0,
        SpriteRenderer,
        Animator,
        Camera,
        Light2D,
        ParticleSystem,
        Rigidbody2D,
        Collider2D,
        BoxCollider2D,
        CircleCollider2D,
        PolygonCollider2D,
        Joint2D,
        AudioSource,
        ScriptComponent,
        UIElement,
        CanvasRenderer,
        TilemapRenderer,
        TerrainRenderer,
        Custom
    };

    // Asset types
    enum class AssetType {
        Texture,
        Sprite,
        Material,
        Shader,
        Script,
        Scene,
        Prefab,
        Audio,
        Font,
        Animation,
        Tilemap,
        Terrain,
        Data
    };

} // namespace CmakeProject1
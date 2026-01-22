#pragma once

#include "Vector2.h"

namespace CmakeProject1 {

    struct Rect {
        float x, y, width, height;
        
        Rect(float x = 0, float y = 0, float width = 0, float height = 0) 
            : x(x), y(y), width(width), height(height) {}
            
        Vector2 position() const { return Vector2(x, y); }
        Vector2 size() const { return Vector2(width, height); }
        Vector2 center() const { return Vector2(x + width/2, y + height/2); }
        
        bool contains(const Vector2& point) const {
            return point.x >= x && point.x < x + width &&
                   point.y >= y && point.y < y + height;
        }
        
        bool intersects(const Rect& other) const {
            return x < other.x + other.width && x + width > other.x &&
                   y < other.y + other.height && y + height > other.y;
        }
    };

} // namespace CmakeProject1
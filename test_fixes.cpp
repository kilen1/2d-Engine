// Test file to verify the fixes to the duplicate type definitions
#include "CMakeProject1/include/Core/Types.h"
#include "CMakeProject1/include/Transform.h"
#include "CMakeProject1/include/Core/Renderer.h"
#include "CMakeProject1/include/Graphics/SpriteBatch.h"
#include "CMakeProject1/include/Graphics/ParticleSystem.h"
#include "CMakeProject1/include/Engine.h"

int main() {
    // Test that Vector2 can be used from different includes
    CmakeProject1::Vector2 v1(1.0f, 2.0f);
    CmakeProject1::Vector2 v2(3.0f, 4.0f);
    auto v3 = v1 + v2;
    
    // Test that Color can be used from different includes
    CmakeProject1::Color c1(1.0f, 0.5f, 0.0f, 1.0f);
    CmakeProject1::Color c2(255, 128, 0, 255); // Uint8 constructor
    CmakeProject1::Color white = CmakeProject1::Color::White;
    
    // Test that Transform can be instantiated
    CmakeProject1::Transform transform(v1);
    
    // Test that Particle can be created
    CmakeProject1::Particle particle;
    particle.position = v1;
    particle.velocity = v2;
    particle.color = c1;
    
    return 0;
}
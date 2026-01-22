// Simple test to verify the fixes to the duplicate type definition issues
// This tests the core fixes without external dependencies

// Previously, including both of these would cause redefinition errors
#include "CMakeProject1/include/Core/Types.h"  // Contains Vector2 and Color
// Transform.h now includes Types.h instead of redefining Vector2
#include "CMakeProject1/include/Transform.h"   // Used to redefine Vector2 causing conflict

// Renderer.h now includes Types.h instead of redefining Color  
#include "CMakeProject1/include/Core/Renderer.h" // Used to redefine Color causing conflict

// These files should now work because they can find Vector2 and Color definitions
#include "CMakeProject1/include/Graphics/SpriteBatch.h"    // Uses Vector2
#include "CMakeProject1/include/Graphics/ParticleSystem.h" // Uses Vector2 and Color
#include "CMakeProject1/include/GraphicsTablet.h"          // Uses Color and defines BlendMode

// Engine.h had incorrect include path that's now fixed
#include "CMakeProject1/include/Engine.h"

int main() {
    // If this compiles, the redefinition issues are fixed
    
    // Test Vector2 from Types.h with operators
    CmakeProject1::Vector2 pos(10.0f, 20.0f);
    CmakeProject1::Vector2 vel(1.0f, 0.5f);
    CmakeProject1::Vector2 newPos = pos + vel;
    
    // Test Color from Types.h with both constructors
    CmakeProject1::Color color1(1.0f, 0.5f, 0.2f, 1.0f);  // Float constructor
    CmakeProject1::Color color2(255, 128, 64, 255);       // Uint8 constructor
    CmakeProject1::Color white = CmakeProject1::Color::White; // Static constant
    
    // Test that Transform can use the unified Vector2
    CmakeProject1::Transform transform(pos);
    
    // Test that Particle can use the unified types
    CmakeProject1::Particle particle;
    particle.position = pos;
    particle.velocity = vel;
    particle.color = color1;
    
    // Test that GraphicsTablet BlendMode is defined
    CmakeProject1::BlendMode mode = CmakeProject1::BlendMode::Normal;
    
    return 0;
}
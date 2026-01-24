cmake_minimum_required(VERSION 3.20)
project(Direct2DEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find DirectX packages
find_package(DirectX REQUIRED)
find_package(WindowsSDK REQUIRED)

# Define source files
set(ENGINE_SOURCES
    src/main.cpp
    src/EngineCore.cpp
    src/GraphicsDevice.cpp
    src/Renderer.cpp
    src/Sprite.cpp
    src/InputManager.cpp
    src/BrushSystem.cpp
    src/PressureBrush.cpp
)

set(ENGINE_HEADERS
    include/EngineCore.h
    include/GraphicsDevice.h
    include/Renderer.h
    include/Sprite.h
    include/InputManager.h
    include/BrushSystem.h
    include/PressureBrush.h
)

# Create main executable
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link DirectX libraries
target_link_libraries(${PROJECT_NAME} 
    d3d11.lib
    dxgi.lib
    d3dcompiler.lib
    winmm.lib
    comctl32.lib
)

# Create DLL library for core engine components
add_library(EngineCoreLib SHARED
    src/EngineCore.cpp
    src/GraphicsDevice.cpp
    src/Renderer.cpp
    include/EngineCore.h
    include/GraphicsDevice.h
    include/Renderer.h
)

target_include_directories(EngineCoreLib PUBLIC include)
target_link_libraries(EngineCoreLib 
    d3d11.lib
    dxgi.lib
    d3dcompiler.lib
)

# Create DLL library for input and brush systems
add_library(InputBrushLib SHARED
    src/InputManager.cpp
    src/BrushSystem.cpp
    src/PressureBrush.cpp
    include/InputManager.h
    include/BrushSystem.h
    include/PressureBrush.h
)

target_include_directories(InputBrushLib PUBLIC include)
target_link_libraries(InputBrushLib 
    d3d11.lib
    dxgi.lib
    d3dcompiler.lib
)
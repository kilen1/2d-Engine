cmake_minimum_required(VERSION 3.20)
project(2DGameEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up vcpkg toolchain
set(VCPKG_ROOT "C:/Users/Zver/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Engine feature options
option(ENGINE_EDITOR "Build with editor" ON)
option(ENGINE_TOOLS "Build with tools" ON)
option(ENGINE_SCRIPTING "Enable scripting support" ON)
option(ENGINE_NETWORKING "Enable networking support" ON)
option(ENGINE_PHYSICS "Enable physics engine" ON)
option(ENGINE_AUDIO "Enable audio engine" ON)
option(ENGINE_UI "Enable UI system" ON)
option(ENGINE_DEBUG "Build with debug symbols" ON)

# Compiler settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

if(ENGINE_AUDIO)
    find_package(SDL2_mixer REQUIRED)
endif()

if(ENGINE_PHYSICS)
    find_package(Box2D REQUIRED)
endif()

if(ENGINE_SCRIPTING)
    find_package(sol2 REQUIRED)
endif()

# Find additional packages that might be available through vcpkg
find_package(nlohmann_json CONFIG)
find_package(spdlog CONFIG)
find_package(fmt CONFIG)
find_package(nativefiledialog-extended CONFIG)
find_package(libzip CONFIG)
find_package(CURL CONFIG)
find_package(OpenSSL CONFIG)
find_package(Steamworks CONFIG)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Audio
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Editor
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imnodes
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/TextEditor
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nativefiledialog-extended/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/entt/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzip/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openssl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/steamworks-sdk
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

if(ENGINE_AUDIO)
    include_directories(${SDL2_MIXER_INCLUDE_DIRS})
endif()

if(ENGINE_PHYSICS)
    include_directories(${BOX2D_INCLUDE_DIRS})
endif()

if(ENGINE_SCRIPTING)
    include_directories(${SOL2_INCLUDE_DIRS})
endif()

include_directories(${OPENGL_INCLUDE_DIRS})

# ImGui library
file(GLOB IMGUI_SOURCES 
    third_party/imgui/*.cpp
    third_party/imgui/backends/imgui_impl_sdl2.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    third_party/imgui
    third_party/imgui/backends
)

# Header-only libraries
add_library(json INTERFACE)
target_include_directories(json INTERFACE third_party/json/include)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE third_party/stb)

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE third_party/entt/src)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE third_party/glm)

# Third-party libraries from local sources
add_library(imnodes STATIC
    third_party/imnodes/imnodes.cpp
    third_party/imnodes/imnodes_internal.h
    third_party/imnodes/imnodes.h
)
target_include_directories(imnodes PUBLIC
    third_party/imnodes
)

add_library(nativefiledialog-extended INTERFACE)
target_include_directories(nativefiledialog-extended INTERFACE
    third_party/nativefiledialog-extended/src/include
)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE third_party/spdlog/include)

add_library(fmt INTERFACE)
target_include_directories(fmt INTERFACE third_party/fmt/include)

add_library(libzip INTERFACE)
target_include_directories(libzip INTERFACE third_party/libzip/lib)

add_library(curl INTERFACE)
target_include_directories(curl INTERFACE third_party/curl/include)

add_library(openssl INTERFACE)
target_include_directories(openssl INTERFACE third_party/openssl/include)

add_library(steamworks INTERFACE)
target_include_directories(steamworks INTERFACE third_party/steamworks-sdk)

# Engine source files
file(GLOB_RECURSE CORE_SOURCES "src/Core/*.cpp")
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
list(APPEND ENGINE_SOURCES ${CORE_SOURCES})

# Additional sources based on features
if(ENGINE_AUDIO)
    file(GLOB_RECURSE AUDIO_SOURCES "src/Audio/*.cpp")
    list(APPEND ENGINE_SOURCES ${AUDIO_SOURCES})
endif()

if(ENGINE_PHYSICS)
    file(GLOB_RECURSE PHYSICS_SOURCES "src/Physics/*.cpp")
    list(APPEND ENGINE_SOURCES ${PHYSICS_SOURCES})
endif()

if(ENGINE_SCRIPTING)
    file(GLOB_RECURSE SCRIPT_SOURCES "src/Scripting/*.cpp")
    list(APPEND ENGINE_SOURCES ${SCRIPT_SOURCES})
endif()

if(ENGINE_EDITOR)
    file(GLOB_RECURSE EDITOR_SOURCES "src/Editor/*.cpp")
    list(APPEND ENGINE_SOURCES ${EDITOR_SOURCES})
endif()

# Main executable
add_executable(2DGameEngine
    ${ENGINE_SOURCES}
    main.cpp
)

# Link libraries
target_link_libraries(2DGameEngine
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${OPENGL_LIBRARIES}
    imgui
    imnodes
    spdlog::spdlog
    fmt::fmt
    nfd
    zip
    curl
    ssl
    crypto
    steam_api
)

if(ENGINE_AUDIO)
    target_link_libraries(2DGameEngine ${SDL2_MIXER_LIBRARIES})
endif()

if(ENGINE_PHYSICS)
    target_link_libraries(2DGameEngine ${BOX2D_LIBRARIES})
endif()

if(ENGINE_SCRIPTING)
    target_link_libraries(2DGameEngine ${SOL2_LIBRARIES})
endif()

# Platform-specific libraries
if(WIN32)
    target_link_libraries(2DGameEngine 
        opengl32 
        gdi32 
        winmm 
        imm32 
        version
    )
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(2DGameEngine X11 Xi Xcursor Xrandr Xinerama Xxf86vm dl m pthread)
elseif(APPLE)
    target_link_libraries(2DGameEngine "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# Compiler definitions
add_definitions(
    -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS
    -DSTB_IMAGE_IMPLEMENTATION
    -DSTB_IMAGE_WRITE_IMPLEMENTATION
)

if(ENGINE_DEBUG)
    target_compile_definitions(2DGameEngine PRIVATE DEBUG=1)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

# Copy assets after build
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets)
add_custom_command(TARGET 2DGameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/bin/assets
)

# Print build configuration
message(STATUS "========================================")
message(STATUS "2D Game Engine Configuration")
message(STATUS "========================================")
message(STATUS "SDL2: YES")
message(STATUS "OpenGL: YES")
message(STATUS "ImGui: Built from source")
message(STATUS "Audio: ${ENGINE_AUDIO}")
message(STATUS "Physics: ${ENGINE_PHYSICS}")
message(STATUS "Scripting: ${ENGINE_SCRIPTING}")
message(STATUS "Editor: ${ENGINE_EDITOR}")
message(STATUS "Networking: ${ENGINE_NETWORKING}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")
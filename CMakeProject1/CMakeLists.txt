cmake_minimum_required(VERSION 3.20)
project(2DGameEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Простые опции - все необязательные функции отключены
option(ENGINE_EDITOR "Build with editor" OFF)
option(ENGINE_TOOLS "Build with tools" OFF)
option(ENGINE_SCRIPTING "Enable scripting support" OFF)
option(ENGINE_NETWORKING "Enable networking support" OFF)  # ОТКЛЮЧЕНО
option(ENGINE_PHYSICS "Enable physics engine" OFF)         # ОТКЛЮЧЕНО
option(ENGINE_AUDIO "Enable audio engine" ON)
option(ENGINE_UI "Enable UI system" ON)
option(ENGINE_DEBUG "Build with debug symbols" ON)

# Настройки сборки
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Поиск только необходимых зависимостей
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

if(ENGINE_AUDIO)
    find_package(SDL2_mixer REQUIRED)
endif()

# Include директории
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

if(ENGINE_AUDIO)
    include_directories(${SDL2_MIXER_INCLUDE_DIRS})
endif()

include_directories(${OPENGL_INCLUDE_DIRS})

# Простая ImGui сборка
file(GLOB IMGUI_SOURCES 
    third_party/imgui/*.cpp
    third_party/imgui/backends/imgui_impl_sdl2.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)

file(GLOB IMGUI_HEADERS
    third_party/imgui/*.h
    third_party/imgui/backends/*.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC
    third_party/imgui
    third_party/imgui/backends
)

# Простые заголовочные библиотеки (без сборки)
add_library(json INTERFACE)
target_include_directories(json INTERFACE third_party/json/include)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE third_party/stb)

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE third_party/entt/src)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE third_party/glm)

# Основные файлы движка
file(GLOB_RECURSE ENGINE_SOURCES 
    "src/*.cpp"
    "src/Core/*.cpp"
    "src/Graphics/*.cpp"
    "src/UI/*.cpp"
)

if(ENGINE_AUDIO)
    file(GLOB_RECURSE AUDIO_SOURCES "src/Audio/*.cpp")
    list(APPEND ENGINE_SOURCES ${AUDIO_SOURCES})
endif()

# Основное приложение
add_executable(2DGameEngine
    ${ENGINE_SOURCES}
    main.cpp
)

# Подключение библиотек
target_link_libraries(2DGameEngine
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${OPENGL_LIBRARIES}
    imgui
)

if(ENGINE_AUDIO)
    target_link_libraries(2DGameEngine ${SDL2_MIXER_LIBRARIES})
endif()

# Настройки для Windows
if(WIN32)
    target_link_libraries(2DGameEngine 
        opengl32 
        gdi32 
        winmm 
        imm32 
        version
    )
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Определения компилятора
add_definitions(
    -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS
    -DSTB_IMAGE_IMPLEMENTATION
    -DSTB_IMAGE_WRITE_IMPLEMENTATION
)

# Копирование ресурсов
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets)
add_custom_command(TARGET 2DGameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/bin/assets
)

message(STATUS "========================================")
message(STATUS "2D Game Engine - Minimal Configuration")
message(STATUS "========================================")
message(STATUS "SDL2: YES")
message(STATUS "OpenGL: YES")
message(STATUS "ImGui: Built from source")
message(STATUS "Audio: ${ENGINE_AUDIO}")
message(STATUS "Networking: DISABLED")
message(STATUS "Physics: DISABLED")
message(STATUS "========================================")
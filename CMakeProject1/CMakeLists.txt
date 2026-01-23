cmake_minimum_required(VERSION 3.20)
project(MyEngine VERSION 1.0.0 LANGUAGES CXX)

# Настройка vcpkg toolchain (укажите свой путь)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Выходные директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Тип сборки (Debug/Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# ======================
# Поиск библиотек через vcpkg
# ======================

# Основные библиотеки
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)

# Дополнительные библиотеки
find_package(entt REQUIRED)
find_package(asio REQUIRED)
find_package(cereal REQUIRED)
find_package(stb REQUIRED)

# Графические библиотеки (опционально)
find_package(freetype)
find_package(glew)
find_package(jsoncpp CONFIG REQUIRED)

# ======================
# Локальные библиотеки (ImGui + ImGuizmo)
# ======================

# Ищем локальные исходники ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
set(IMGUIZMO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imguizmo)

# Проверяем наличие ImGui
if(NOT EXISTS ${IMGUI_DIR}/imgui.h)
    message(WARNING "ImGui not found in third_party/imgui")
    # Можно добавить загрузку через FetchContent
    include(FetchContent)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
    )
    FetchContent_MakeAvailable(imgui)
    set(IMGUI_DIR ${imgui_SOURCE_DIR})
endif()

# Проверяем наличие ImGuizmo
if(NOT EXISTS ${IMGUIZMO_DIR}/ImGuizmo.h)
    message(WARNING "ImGuizmo not found in third_party/imguizmo")
    include(FetchContent)
    FetchContent_Declare(
        imguizmo
        GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(imguizmo)
    set(IMGUIZMO_DIR ${imguizmo_SOURCE_DIR})
endif()

# Создаем статическую библиотеку ImGui
file(GLOB IMGUI_SOURCES 
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Подключаем зависимости для ImGui
target_link_libraries(imgui 
    SDL2::SDL2
    OpenGL::GL
)

# Компиляционные определения для ImGui
target_compile_definitions(imgui PRIVATE
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
)

# Создаем статическую библиотеку ImGuizmo
add_library(imguizmo STATIC ${IMGUIZMO_DIR}/ImGuizmo.cpp)
target_include_directories(imguizmo PUBLIC ${IMGUIZMO_DIR})
target_link_libraries(imguizmo imgui)

# ======================
# Основной проект
# ======================

# Находим все исходные файлы
file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp
)

# Находим все заголовочные файлы
file(GLOB_RECURSE HEADER_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*/*.h
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/main.cpp)

# Директории include
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUIZMO_DIR}
    ${IMGUI_DIR}/backends
)

# Линковка библиотек
target_link_libraries(${PROJECT_NAME}
    # Локальные библиотеки
    imgui
    imguizmo
    
    # Vcpkg библиотеки
    SDL2::SDL2
    glm::glm
    OpenGL::GL
    spdlog::spdlog
    fmt::fmt
    nlohmann_json::nlohmann_json
    EnTT::EnTT
    asio::asio
    cereal::cereal
    jsoncpp_lib
    SDL2_image::SDL2_image   
    SDL2_mixer::SDL2_mixer   
    SDL2_ttf::SDL2_ttf  
    SDL2::SDL2main
)

# Windows специфичные настройки
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        opengl32
        gdi32
        winmm
        imm32
        version
        shell32  # Для некоторых функций
    )
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
    
    # Настройки для MSVC
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
            /W4
            /wd4201  # Игнорировать nonstandard extension used: nameless struct/union
            /wd4127  # Игнорировать conditional expression is constant
            /MP      # Параллельная компиляция
        )
        
        # Настройки для отладки
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(${PROJECT_NAME} PRIVATE /Zi)
            target_link_options(${PROJECT_NAME} PRIVATE /DEBUG)
        endif()
    endif()
endif()

# Компиляционные определения
target_compile_definitions(${PROJECT_NAME} PRIVATE
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
)

# Настройки сборки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _DEBUG
        ENGINE_DEBUG=1
    )
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /Zi)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -g)
    endif()
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        NDEBUG
    )
    target_compile_options(${PROJECT_NAME} PRIVATE -O2)
endif()

# Копирование ресурсов после сборки
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        COMMENT "Copying assets..."
    )
endif()

# Копирование DLL файлов (для Windows)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL2::SDL2>"
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copying SDL2 DLL..."
    )
endif()

# ======================
# Информация о конфигурации
# ======================

message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "========================================")
message(STATUS "Libraries:")
message(STATUS "  ImGui: ${IMGUI_DIR}")
message(STATUS "  ImGuizmo: ${IMGUIZMO_DIR}")
message(STATUS "  SDL2: Found")
message(STATUS "  OpenGL: Found")
message(STATUS "  glm: Found")
message(STATUS "========================================")
message(STATUS "Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "========================================")
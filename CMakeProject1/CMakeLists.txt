cmake_minimum_required(VERSION 3.20)
project(CmakeProject1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка vcpkg toolchain (измените путь если нужно)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/Zver/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Опции движка
option(ENGINE_AUDIO "Enable audio support" ON)
option(ENGINE_PHYSICS "Enable physics support" ON)
option(ENGINE_SCRIPTING "Enable scripting support" ON)
option(ENGINE_NETWORKING "Enable networking support" ON)
option(ENGINE_EDITOR "Enable editor" ON)

# Пути для выходных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Найти пакеты через vcpkg
find_package(OpenGL REQUIRED)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)
find_package(sdl2_ttf REQUIRED)

if(ENGINE_AUDIO)
    find_package(sdl2_mixer REQUIRED)
endif()

if(ENGINE_SCRIPTING)
    find_package(sol2 REQUIRED)
endif()

# Дополнительные библиотеки из vcpkg
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(nfd REQUIRED)
find_package(libzip REQUIRED)
find_package(CURL REQUIRED)
find_package(openssl REQUIRED)
find_package(stb REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)
find_package(entt REQUIRED)
find_package(glm REQUIRED)

# Steamworks опционально
find_package(Steamworks)

# Физика Box2D
if(ENGINE_PHYSICS)
    find_package(Box2D REQUIRED)
endif()

# Сеть
if(ENGINE_NETWORKING)
    # ENET поиск
    find_path(ENET_INCLUDE_DIR enet/enet.h
        PATHS "C:/Users/Zver/vcpkg/installed/x64-windows/include"
        "C:/Users/Zver/vcpkg/installed/x86-windows/include"
        NO_DEFAULT_PATH
    )

    find_library(ENET_LIBRARY_RELEASE
        NAMES enet
        PATHS "C:/Users/Zver/vcpkg/installed/x64-windows/lib"
        "C:/Users/Zver/vcpkg/installed/x86-windows/lib"
        NO_DEFAULT_PATH
    )

    find_library(ENET_LIBRARY_DEBUG
        NAMES enetd enet
        PATHS "C:/Users/Zver/vcpkg/installed/x64-windows/debug/lib"
        "C:/Users/Zver/vcpkg/installed/x86-windows/debug/lib"
        NO_DEFAULT_PATH
    )

    # WebSocket++ поиск
    find_path(WEBSOCKETPP_INCLUDE_DIR websocketpp/config/asio_no_tls.hpp
        PATHS "C:/Users/Zver/vcpkg/installed/x64-windows/include"
        "C:/Users/Zver/vcpkg/installed/x86-windows/include"
        /usr/include
        /usr/local/include
    )

    if(NOT WEBSOCKETPP_INCLUDE_DIR)
        include(FetchContent)
        FetchContent_Declare(
            websocketpp
            GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
            GIT_TAG 0.8.2
        )
        FetchContent_MakeAvailable(websocketpp)
        set(WEBSOCKETPP_INCLUDE_DIR ${websocketpp_SOURCE_DIR})
    endif()
endif()

# ======================
# Импортированные библиотеки
# ======================

# Создаем импортированную цель для enet если найдена
if(ENET_INCLUDE_DIR AND (ENET_LIBRARY_RELEASE OR ENET_LIBRARY_DEBUG))
    add_library(enet::enet UNKNOWN IMPORTED)
    set_target_properties(enet::enet PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${ENET_INCLUDE_DIR}"
    )
    
    if(ENET_LIBRARY_RELEASE)
        set_property(TARGET enet::enet APPEND PROPERTY
            IMPORTED_CONFIGURATIONS RELEASE
        )
        set_target_properties(enet::enet PROPERTIES
            IMPORTED_LOCATION_RELEASE "${ENET_LIBRARY_RELEASE}"
        )
    endif()
    
    if(ENET_LIBRARY_DEBUG)
        set_property(TARGET enet::enet APPEND PROPERTY
            IMPORTED_CONFIGURATIONS DEBUG
        )
        set_target_properties(enet::enet PROPERTIES
            IMPORTED_LOCATION_DEBUG "${ENET_LIBRARY_DEBUG}"
        )
    endif()
endif()

# ======================
# Локальные библиотеки (только imgui и imnodes)
# ======================

# ImGui library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui.cpp)
    file(GLOB IMGUI_SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp
    )
    
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends
    )
    
    # Добавляем SDL2 include к imgui
    if(SDL2_INCLUDE_DIRS)
        target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})
    elseif(sdl2_PREFIX)
        target_include_directories(imgui PUBLIC ${sdl2_PREFIX}/include)
    else()
        find_path(SDL2_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2)
        if(SDL2_INCLUDE_DIR)
            target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIR})
        endif()
    endif()
    
    target_compile_definitions(imgui PRIVATE -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
    message(STATUS "ImGui: Building from local source")
else()
    message(FATAL_ERROR "ImGui not found in third_party folder!")
endif()

# ImNodes library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imnodes/imnodes.cpp)
    add_library(imnodes STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imnodes/imnodes.cpp
    )
    target_include_directories(imnodes PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imnodes
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
    )
    
    target_link_libraries(imnodes PUBLIC imgui)
    target_compile_definitions(imnodes PRIVATE -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
    
    message(STATUS "ImNodes: Building from local source")
else()
    message(FATAL_ERROR "ImNodes not found in third_party folder!")
endif()

# TextEditor library (если есть)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/TextEditor/TextEditor.cpp)
    add_library(TextEditor STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/TextEditor/TextEditor.cpp
    )
    target_include_directories(TextEditor PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/TextEditor
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
    )
    
    target_link_libraries(TextEditor PUBLIC imgui)
    message(STATUS "TextEditor: Building from local source")
endif()

# ======================
# Основной движок
# ======================

# Собираем список исходных файлов движка
set(ENGINE_SOURCES
    src/main.cpp
    src/Engine.cpp
    src/Editor.cpp
)

# Находим все .cpp файлы в src (кроме main.cpp, Engine.cpp, Editor.cpp)
file(GLOB_RECURSE SRC_CPP_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*.cpp
)

# Убираем уже добавленные файлы
list(REMOVE_ITEM SRC_CPP_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp
)

# Добавляем оставшиеся файлы
list(APPEND ENGINE_SOURCES ${SRC_CPP_FILES})

# Основной исполняемый файл
add_executable(CmakeProject1 ${ENGINE_SOURCES})

# Настройка include директорий
target_include_directories(CmakeProject1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imnodes
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/TextEditor
    $<$<BOOL:${ENET_INCLUDE_DIR}>:${ENET_INCLUDE_DIR}>
    $<$<BOOL:${WEBSOCKETPP_INCLUDE_DIR}>:${WEBSOCKETPP_INCLUDE_DIR}>
)

# Линковка библиотек
target_link_libraries(CmakeProject1
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    OpenGL::GL
    imgui
    imnodes
    spdlog::spdlog
    fmt::fmt
    nlohmann_json::nlohmann_json
    nfd::nfd
    libzip::zip
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    EnTT::EnTT
    glm::glm
    asio::asio
    cereal::cereal
)

# Дополнительные библиотеки
if(ENGINE_AUDIO)
    target_link_libraries(CmakeProject1 SDL2_mixer::SDL2_mixer)
endif()

if(ENGINE_PHYSICS)
    target_link_libraries(CmakeProject1 box2d::box2d)
endif()

if(ENGINE_SCRIPTING)
    target_link_libraries(CmakeProject1 sol2::sol2)
endif()

if(TARGET TextEditor)
    target_link_libraries(CmakeProject1 TextEditor)
endif()

if(TARGET enet::enet)
    target_link_libraries(CmakeProject1 enet::enet)
endif()

if(Steamworks_FOUND)
    target_link_libraries(CmakeProject1 Steamworks::Steamworks)
endif()

# Платформозависимые библиотеки
if(WIN32)
    target_link_libraries(CmakeProject1 
        opengl32 
        gdi32 
        winmm 
        imm32 
        version
    )
    target_compile_definitions(CmakeProject1 PRIVATE 
        -D_CRT_SECURE_NO_WARNINGS
        -DNOMINMAX
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(CmakeProject1 X11 Xi Xcursor Xrandr Xinerama Xxf86vm dl m pthread)
elseif(APPLE)
    target_link_libraries(CmakeProject1 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreVideo"
        "-framework OpenGL"
    )
endif()

# Компиляционные определения
target_compile_definitions(CmakeProject1 
    PRIVATE
    -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS
    -DGLM_ENABLE_EXPERIMENTAL
)

# Настройки режима сборки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(CmakeProject1 PRIVATE 
        ENGINE_DEBUG=1
        _DEBUG=1
    )
    target_compile_options(CmakeProject1 PRIVATE -g -O0)
else()
    target_compile_definitions(CmakeProject1 PRIVATE 
        NDEBUG=1
    )
    target_compile_options(CmakeProject1 PRIVATE -O3 -DNDEBUG)
endif()

# Копирование ресурсов после сборки
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets)
    add_custom_command(TARGET CmakeProject1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/bin/assets
        COMMENT "Copying assets..."
    )
endif()

# Создание папки конфигураций
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/config)
    add_custom_command(TARGET CmakeProject1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_BINARY_DIR}/bin/config
        COMMENT "Copying config files..."
    )
endif()

# Отображение конфигурации сборки
message(STATUS "========================================")
message(STATUS "2D Game Engine Configuration")
message(STATUS "========================================")
message(STATUS "Project: CmakeProject1")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")
message(STATUS "Libraries:")
message(STATUS "  SDL2: YES")
message(STATUS "  OpenGL: YES")
message(STATUS "  ImGui: Local source")
message(STATUS "  ImNodes: Local source")
message(STATUS "  TextEditor: $<IF:$<TARGET_EXISTS:TextEditor>,YES,NO>")
message(STATUS "  Spdlog: Vcpkg")
message(STATUS "  nlohmann_json: Vcpkg")
message(STATUS "  glm: Vcpkg")
message(STATUS "  EnTT: Vcpkg")
message(STATUS "  asio: Vcpkg")
message(STATUS "  cereal: Vcpkg")
message(STATUS "========================================")
message(STATUS "Features:")
message(STATUS "  Audio: ${ENGINE_AUDIO}")
message(STATUS "  Physics: ${ENGINE_PHYSICS}")
message(STATUS "  Scripting: ${ENGINE_CRIPTING}")
message(STATUS "  Networking: ${ENGINE_NETWORKING}")
message(STATUS "  Editor: ${ENGINE_EDITOR}")
message(STATUS "========================================")
message(STATUS "Output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "========================================")

# Установка по желанию
install(TARGETS CmakeProject1
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

install(DIRECTORY assets DESTINATION .)
install(DIRECTORY config DESTINATION .)
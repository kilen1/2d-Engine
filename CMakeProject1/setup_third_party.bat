@echo off
setlocal enabledelayedexpansion

echo Setting up third-party libraries in third_party directory...
set "PROJECT_ROOT=D:\CMakeProject1\CMakeProject1"
set "THIRD_PARTY_DIR=%PROJECT_ROOT%\third_party"

:: Create third_party directory if it doesn't exist
if not exist "%THIRD_PARTY_DIR%" mkdir "%THIRD_PARTY_DIR%"

:: Change to third party directory
cd /d "%THIRD_PARTY_DIR%"

echo Downloading and setting up libraries...

:: 1. imnodes (Dear ImGui Nodes)
echo Installing imnodes...
if not exist "imnodes" (
    git clone https://github.com/Nelarius/imnodes.git
) else (
    echo imnodes already exists, updating...
    cd imnodes
    git pull
    cd ..
)

:: 2. TextEditor (custom header file for text editing functionality)
echo Creating TextEditor directory...
if not exist "TextEditor" mkdir "TextEditor"
:: We'll need to either find a suitable TextEditor implementation or create our own
:: For now, create a placeholder
echo // Placeholder for TextEditor.h > "TextEditor\TextEditor.h"
echo #pragma once >> "TextEditor\TextEditor.h"
echo. >> "TextEditor\TextEditor.h"
echo // Add your TextEditor implementation here >> "TextEditor\TextEditor.h"

:: 3. Native File Dialog Extended
echo Installing nativefiledialog-extended...
if not exist "nativefiledialog-extended" (
    git clone https://github.com/btzy/nativefiledialog-extended.git
) else (
    echo nativefiledialog-extended already exists, updating...
    cd nativefiledialog-extended
    git pull
    cd ..
)

:: 4. JSON (nlohmann/json) - already in vcpkg but creating local copy
echo Installing nlohmann/json...
if not exist "json" (
    git clone https://github.com/nlohmann/json.git
    :: Create proper directory structure
    if not exist "json\include" mkdir "json\include"
    xcopy /E /I "json\include" "json\include\json"
    ren "json\include\json\include\nlohmann" "json\include\json"
) else (
    echo nlohmann/json already exists, updating...
    cd json
    git pull
    cd ..
)

:: 5. EnTT - already in vcpkg but creating local copy
echo Installing EnTT...
if not exist "entt" (
    git clone https://github.com/skypjack/entt.git
) else (
    echo EnTT already exists, updating...
    cd entt
    git pull
    cd ..
)

:: 6. spdlog - already in vcpkg but creating local copy
echo Installing spdlog...
if not exist "spdlog" (
    git clone https://github.com/gabime/spdlog.git
) else (
    echo spdlog already exists, updating...
    cd spdlog
    git pull
    cd ..
)

:: 7. fmt - already in vcpkg but creating local copy
echo Installing fmt...
if not exist "fmt" (
    git clone https://github.com/fmtlib/fmt.git
) else (
    echo fmt already exists, updating...
    cd fmt
    git pull
    cd ..
)

:: 8. libzip
echo Installing libzip...
if not exist "libzip" (
    git clone https://github.com/nih-at/libzip.git
) else (
    echo libzip already exists, updating...
    cd libzip
    git pull
    cd ..
)

:: 9. libcurl
echo Installing libcurl...
if not exist "curl" (
    git clone https://github.com/curl/curl.git
) else (
    echo curl already exists, updating...
    cd curl
    git pull
    cd ..
)

:: 10. OpenSSL
echo Installing OpenSSL...
if not exist "openssl" (
    git clone https://github.com/openssl/openssl.git
) else (
    echo openssl already exists, updating...
    cd openssl
    git pull
    cd ..
)

:: 11. Steamworks SDK
echo Installing Steamworks SDK...
if not exist "steamworks-sdk" (
    echo Please manually download Steamworks SDK from https://partner.steamgames.com/
    echo Place the contents in a 'steamworks-sdk' folder in this directory.
    echo Creating placeholder directory...
    mkdir "steamworks-sdk"
    echo // Placeholder for steam API headers > "steamworks-sdk\steam_api.h"
) else (
    echo Steamworks SDK already exists in steamworks-sdk folder
)

:: Additional setup for some libraries
echo Setting up include directories...

:: Make sure proper directory structure exists
if not exist "json\include" mkdir "json\include"
if not exist "spdlog\include" mkdir "spdlog\include"
if not exist "fmt\include" mkdir "fmt\include"
if not exist "curl\include" mkdir "curl\include"
if not exist "openssl\include" mkdir "openssl\include"

echo Libraries setup complete!
echo Remember to update your CMakeLists.txt to include these libraries.
echo Also ensure your project is configured to use vcpkg as well.

pause
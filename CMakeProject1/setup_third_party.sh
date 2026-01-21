#!/bin/bash

echo "Setting up third-party libraries in third_party directory..."
PROJECT_ROOT="/workspace/CMakeProject1"
THIRD_PARTY_DIR="$PROJECT_ROOT/third_party"

# Create third_party directory if it doesn't exist
mkdir -p "$THIRD_PARTY_DIR"

# Change to third party directory
cd "$THIRD_PARTY_DIR" || exit 1

echo "Downloading and setting up libraries..."

# 1. imnodes (Dear ImGui Nodes)
echo "Installing imnodes..."
if [ ! -d "imnodes" ]; then
    git clone https://github.com/Nelarius/imnodes.git
else
    echo "imnodes already exists, updating..."
    cd imnodes
    git pull
    cd ..
fi

# 2. TextEditor (custom header file for text editing functionality)
echo "Creating TextEditor directory..."
if [ ! -d "TextEditor" ]; then
    mkdir -p "TextEditor"
    # Create a placeholder TextEditor.h
    cat > "TextEditor/TextEditor.h" << 'EOF'
#pragma once

// Placeholder for TextEditor.h
// Add your TextEditor implementation here
EOF
fi

# 3. Native File Dialog Extended
echo "Installing nativefiledialog-extended..."
if [ ! -d "nativefiledialog-extended" ]; then
    git clone https://github.com/btzy/nativefiledialog-extended.git
else
    echo "nativefiledialog-extended already exists, updating..."
    cd nativefiledialog-extended
    git pull
    cd ..
fi

# 4. JSON (nlohmann/json)
echo "Installing nlohmann/json..."
if [ ! -d "json" ]; then
    git clone https://github.com/nlohmann/json.git
    # Create proper directory structure
    if [ ! -d "json/include" ]; then
        mkdir -p "json/include/json"
    fi
    cp -r json/single_include/nlohmann/* json/include/json/ 2>/dev/null || echo "Using alternative directory structure for json"
else
    echo "nlohmann/json already exists, updating..."
    cd json
    git pull
    cd ..
fi

# 5. EnTT
echo "Installing EnTT..."
if [ ! -d "entt" ]; then
    git clone https://github.com/skypjack/entt.git
else
    echo "EnTT already exists, updating..."
    cd entt
    git pull
    cd ..
fi

# 6. spdlog
echo "Installing spdlog..."
if [ ! -d "spdlog" ]; then
    git clone https://github.com/gabime/spdlog.git
else
    echo "spdlog already exists, updating..."
    cd spdlog
    git pull
    cd ..
fi

# 7. fmt
echo "Installing fmt..."
if [ ! -d "fmt" ]; then
    git clone https://github.com/fmtlib/fmt.git
else
    echo "fmt already exists, updating..."
    cd fmt
    git pull
    cd ..
fi

# 8. libzip
echo "Installing libzip..."
if [ ! -d "libzip" ]; then
    git clone https://github.com/nih-at/libzip.git
else
    echo "libzip already exists, updating..."
    cd libzip
    git pull
    cd ..
fi

# 9. libcurl
echo "Installing libcurl..."
if [ ! -d "curl" ]; then
    git clone https://github.com/curl/curl.git
else
    echo "curl already exists, updating..."
    cd curl
    git pull
    cd ..
fi

# 10. OpenSSL
echo "Installing OpenSSL..."
if [ ! -d "openssl" ]; then
    git clone https://github.com/openssl/openssl.git
else
    echo "openssl already exists, updating..."
    cd openssl
    git pull
    cd ..
fi

# 11. Steamworks SDK
echo "Installing Steamworks SDK..."
if [ ! -d "steamworks-sdk" ]; then
    echo "Please manually download Steamworks SDK from https://partner.steamgames.com/"
    echo "Place the contents in a 'steamworks-sdk' folder in this directory."
    echo "Creating placeholder directory..."
    mkdir -p "steamworks-sdk"
    cat > "steamworks-sdk/steam_api.h" << 'EOF'
// Placeholder for steam API headers
EOF
else
    echo "Steamworks SDK already exists in steamworks-sdk folder"
fi

# Additional setup for some libraries
echo "Setting up include directories..."

# Make sure proper directory structure exists
mkdir -p "json/include"
mkdir -p "spdlog/include"
mkdir -p "fmt/include"
mkdir -p "curl/include"
mkdir -p "openssl/include"

echo "Libraries setup complete!"
echo "Remember to update your CMakeLists.txt to include these libraries."
echo "Also ensure your project is configured to use vcpkg as well."
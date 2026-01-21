# Third-Party Library Setup Guide

This guide explains how to set up and use the third-party libraries for the CMake project.

## Libraries Included

The following libraries are configured for this project:

1. **imnodes** - Node editor for Dear ImGui
   - Repository: https://github.com/Nelarius/imnodes
   - Headers: `#include "imnodes.h"`

2. **TextEditor** - Custom text editor component
   - Headers: `#include "TextEditor.h"`

3. **Native File Dialog Extended** - Cross-platform file dialogs
   - Repository: https://github.com/btzy/nativefiledialog-extended
   - Headers: `#include <nfd.h>`

4. **nlohmann/json** - JSON for Modern C++
   - Repository: https://github.com/nlohmann/json
   - Headers: `#include <json/json.h>`

5. **EnTT** - Gaming/Entity-Component-System framework
   - Repository: https://github.com/skypjack/entt
   - Headers: `#include <entt/entt.hpp>`

6. **spdlog** - Fast C++ logging library
   - Repository: https://github.com/gabime/spdlog
   - Headers: `#include <spdlog/spdlog.h>` and related

7. **fmt** - Open-source formatting library
   - Repository: https://github.com/fmtlib/fmt
   - Headers: `#include <fmt/format.h>`

8. **libzip** - Library for reading, creating, and modifying zip archives
   - Repository: https://github.com/nih-at/libzip
   - Headers: `#include <zip.h>`

9. **libcurl** - URL transfer library
   - Repository: https://github.com/curl/curl
   - Headers: `#include <curl/curl.h>`

10. **OpenSSL** - Cryptography and SSL/TLS toolkit
    - Repository: https://github.com/openssl/openssl
    - Headers: `#include <openssl/sha.h>`

11. **Steamworks SDK** - Valve's Steam integration library
    - Manual download required from https://partner.steamgames.com/
    - Headers: `#include <steam/steam_api.h>`

## Setup Instructions

### Windows Batch Script

A batch script is provided to automate the setup process:

```
setup_third_party.bat
```

This script will:
- Clone or update all required repositories
- Create proper directory structures
- Provide instructions for manually downloaded libraries

### Manual Setup

If you prefer to set up libraries manually:

1. Create the `third_party` directory in your project root
2. Clone or download each library to its respective subdirectory under `third_party`
3. Ensure the directory structure matches what's expected in CMakeLists.txt

## CMake Configuration

The CMakeLists.txt file is configured to:
- Include all necessary header directories
- Build static libraries where needed (like imnodes)
- Link against all required libraries
- Use vcpkg for package management (configured to use C:/Users/Zver/vcpkg)

## Using the Libraries

After running the setup script and building the project, you can use the libraries as follows:

```cpp
#include "imnodes.h"
#include "TextEditor.h"
#include <nfd.h>
#include <json/json.h>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <zip.h>
#include <curl/curl.h>
#include <openssl/sha.h>
#include <steam/steam_api.h>
```

## Notes

- Some libraries (like Steamworks SDK) require manual download due to licensing restrictions
- The CMake configuration prioritizes local copies in third_party but will fall back to vcpkg versions when available
- Make sure to update your vcpkg installation regularly to keep dependencies up to date
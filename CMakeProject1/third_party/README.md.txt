# Third-Party Dependencies

## Required Libraries

1. **SDL2** - https://www.libsdl.org/
2. **SDL2_image** - https://www.libsdl.org/projects/SDL_image/
3. **SDL2_ttf** - https://www.libsdl.org/projects/SDL_ttf/
4. **SDL2_mixer** - https://www.libsdl.org/projects/SDL_mixer/
5. **Lua 5.4** - https://www.lua.org/

## Optional Libraries

6. **ENet** - http://enet.bespin.org/
7. **WebSocket++** - https://github.com/zaphoyd/websocketpp
8. **asio** - https://think-async.com/Asio/
9. **Box2D** - https://box2d.org/
10. **SQLite3** - https://www.sqlite.org/

## Header-Only Libraries (included in repo)

Place these in `third_party/` directory:

- **glm** - https://github.com/g-truc/glm
- **nlohmann/json** - https://github.com/nlohmann/json
- **spdlog** - https://github.com/gabime/spdlog
- **stb** - https://github.com/nothings/stb
- **entt** - https://github.com/skypjack/entt
- **sol2** - https://github.com/ThePhD/sol2
- **imgui** - https://github.com/ocornut/imgui

## Quick Setup Script (Linux/Mac)

```bash
#!/bin/bash

# Create third_party directory
mkdir -p third_party

# Clone header-only libraries
git clone https://github.com/g-truc/glm.git third_party/glm
git clone https://github.com/nlohmann/json.git third_party/json
git clone https://github.com/gabime/spdlog.git third_party/spdlog
git clone https://github.com/nothings/stb.git third_party/stb
git clone https://github.com/skypjack/entt.git third_party/entt
git clone https://github.com/ThePhD/sol2.git third_party/sol2
git clone https://github.com/ocornut/imgui.git third_party/imgui

# Clone other libraries
git clone https://github.com/lsalzman/enet.git third_party/enet
git clone https://github.com/zaphoyd/websocketpp.git third_party/websocketpp
git clone https://github.com/chriskohlhoff/asio.git third_party/asio
git clone https://github.com/erincatto/box2d.git third_party/box2d

# Download SQLite3 amalgamation
wget https://www.sqlite.org/2023/sqlite-amalgamation-3410200.zip -O third_party/sqlite3.zip
unzip third_party/sqlite3.zip -d third_party/sqlite3
rm third_party/sqlite3.zip

echo "Dependencies downloaded successfully!"
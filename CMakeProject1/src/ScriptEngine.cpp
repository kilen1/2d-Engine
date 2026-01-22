#include "../include/ScriptEngine.h"
#include "../include/Engine.h"
#include "../include/GameObject.h"
#include "../include/Transform.h"
#include "../include/SpriteRenderer.h"
#include "../include/Graphics/Camera.h"
#include "../include/Rigidbody2D.h"
#include "../include/Collider2D.h"
#include "../include/Audio/AudioSource.h"
#include "../include/Graphics/ParticleSystem.h"
#include "../include/Animation/AnimationSystem.h"
#include "../include/UI/UIManager.h"
#include "../include/core/InputManager.h"
#include "../include/core/TimeManager.h"
#include "../include/Scene/SceneManager.h"
#include "../include/core/AssetManager.h"
#include "../include/core/PhysicsEngine.h"
#include "../include/Core/AudioEngine.h"
#include "../include/NetworkManager.h"
#include "../include/core/EventSystem.h"
#include "../include/Tools/DebugDraw.h"
#include "../include/Tools/Console.h"
#include "../include/Tools/Profiler.h"
#include "../include/LocalizationManager.h"
#include "../include/SaveSystem.h"
#include "../include/QuestSystem.h"
#include "../include/InventorySystem.h"
#include "../include/DialogSystem.h"
#include "../include/AISystem.h"
#include "../include/Serialization/PrefabSystem.h"
#include "../include/ModdingSystem.h"

#define SOL_ALL_SAFETIES_ON 1
#define SOL_LUA_VERSION_I_ 504  // Define Lua 5.4
#include <sol/sol.hpp>
#include <lua.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>

// Lua библиотеки
extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}

namespace fs = std::filesystem;

namespace CmakeProject1 {

    // Singleton implementation
    ScriptEngine& ScriptEngine::getInstance() {
        static ScriptEngine instance;
        return instance;
    }

    ScriptEngine::ScriptEngine() 
        : nextListenerId(1), 
          debuggerEnabled(false), 
          sandboxEnabled(true),
          jitEnabled(false) {
        
        // Создание Lua состояния
        lua = sol::state();
        lua.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            sol::lib::os,
            sol::lib::math,
            sol::lib::table,
            sol::lib::debug,
            sol::lib::bit32,
            sol::lib::io,
            sol::lib::ffi,
            sol::lib::jit,
            sol::lib::utf8
        );
        
        // Инициализация LuaJIT если доступен
        #ifdef LUAJIT_VERSION
        lua["jit"].get<sol::table>()["on"] = []() { };
        lua["jit"].get<sol::table>()["off"] = []() { };
        #endif
    }

    ScriptEngine::~ScriptEngine() {
        shutdown();
    }

    bool ScriptEngine::initialize() {
        try {
            SPDLOG_INFO("Initializing script engine...");
            
            // Настройка package.path для поиска скриптов
            std::string packagePath = lua["package"]["path"];
            packagePath += ";./Scripts/?.lua";
            packagePath += ";./Assets/Scripts/?.lua";
            packagePath += ";./Scenes/?.lua";
            packagePath += ";./?.lua";
            packagePath += ";./lib/?.lua";
            
            lua["package"]["path"] = packagePath;
            
            // Настройка package.cpath для C модулей
            std::string packageCpath = lua["package"]["cpath"];
            #ifdef _WIN32
                packageCpath += ";./lib/?.dll";
                packageCpath += ";./?.dll";
            #elif defined(__linux__)
                packageCpath += ";./lib/?.so";
                packageCpath += ";./?.so";
            #elif defined(__APPLE__)
                packageCpath += ";./lib/?.dylib";
                packageCpath += ";./?.dylib";
            #endif
            
            lua["package"]["cpath"] = packageCpath;
            
            // Регистрация стандартных библиотек
            registerStandardLibrary();
            
            // Регистрация API движка
            registerEngineAPI();
            
            // Настройка окружения для песочницы
            if (sandboxEnabled) {
                setupSandbox();
            }
            
            // Загрузка базовых модулей движка
            loadCoreModules();
            
            // Инициализация отладчика если нужно
            if (debuggerEnabled) {
                initializeDebugger();
            }
            
            SPDLOG_INFO("Script engine initialized successfully");
            return true;
            
        } catch (const sol::error& e) {
            SPDLOG_ERROR("Script engine initialization failed: {}", e.what());
            return false;
        }
    }

    void ScriptEngine::shutdown() {
        SPDLOG_INFO("Shutting down script engine...");
        
        // Остановка всех потоков
        joinAllThreads();
        
        // Отключение от IDE
        if (ideConnected) {
            disconnectFromIDE();
        }
        
        // Сохранение состояния скриптов
        saveScriptStates();
        
        // Очистка всех ресурсов
        scriptRegistry.clear();
        scriptInstances.clear();
        loadedPackages.clear();
        scriptModificationTimes.clear();
        scriptsToReload.clear();
        eventListeners.clear();
        consoleCommands.clear();
        coroutines.clear();
        
        // Очистка Lua состояния
        lua.collect_garbage();
        
        SPDLOG_INFO("Script engine shutdown complete");
    }

    void ScriptEngine::update(float deltaTime) {
        try {
            // Обновление всех активных скриптов
            for (auto& script : scriptInstances) {
                if (script->isEnabled()) {
                    script->update(deltaTime);
                }
            }
            
            // Обновление корутин
            updateCoroutines(deltaTime);
            
            // Обновление горячей перезагрузки
            checkForHotReload();
            
            // Обновление отладчика
            if (debuggerEnabled) {
                updateDebugger();
            }
            
            // Обновление профилирования
            if (profilingEnabled) {
                updateProfiling(deltaTime);
            }
            
        } catch (const sol::error& e) {
            SPDLOG_ERROR("Script engine update error: {}", e.what());
        }
    }

    void ScriptEngine::registerStandardLibrary() {
        // Математические функции
        registerMathLibrary();
        
        // Строковые функции
        registerStringLibrary();
        
        // Табличные функции
        registerTableLibrary();
        
        // OS функции (ограниченные в песочнице)
        registerOSLibrary();
        
        // IO функции (ограниченные в песочнице)
        registerIOLibrary();
        
        // Отладочные функции
        registerDebugLibrary();
        
        // Coroutine функции
        registerCoroutineLibrary();
    }

    void ScriptEngine::registerMathLibrary() {
        auto math = lua["math"];
        
        // Константы
        math["pi"] = 3.14159265358979323846;
        math["huge"] = HUGE_VAL;
        
        // Тригонометрические функции
        math["sin"] = [](double x) { return std::sin(x); };
        math["cos"] = [](double x) { return std::cos(x); };
        math["tan"] = [](double x) { return std::tan(x); };
        math["asin"] = [](double x) { return std::asin(x); };
        math["acos"] = [](double x) { return std::acos(x); };
        math["atan"] = [](double x) { return std::atan(x); };
        math["atan2"] = [](double y, double x) { return std::atan2(y, x); };
        
        // Гиперболические функции
        math["sinh"] = [](double x) { return std::sinh(x); };
        math["cosh"] = [](double x) { return std::cosh(x); };
        math["tanh"] = [](double x) { return std::tanh(x); };
        
        // Экспоненциальные и логарифмические
        math["exp"] = [](double x) { return std::exp(x); };
        math["log"] = [](double x) { return std::log(x); };
        math["log10"] = [](double x) { return std::log10(x); };
        
        // Степенные
        math["pow"] = [](double x, double y) { return std::pow(x, y); };
        math["sqrt"] = [](double x) { return std::sqrt(x); };
        
        // Округление
        math["floor"] = [](double x) { return std::floor(x); };
        math["ceil"] = [](double x) { return std::ceil(x); };
        math["round"] = [](double x) { return std::round(x); };
        math["trunc"] = [](double x) { return std::trunc(x); };
        
        // Абсолютное значение
        math["abs"] = [](double x) { return std::abs(x); };
        
        // Минимум/максимум
        math["min"] = sol::variadic_args([](sol::variadic_args args) {
            double minVal = args[0].as<double>();
            for (auto v : args) {
                double val = v.as<double>();
                if (val < minVal) minVal = val;
            }
            return minVal;
        });
        
        math["max"] = sol::variadic_args([](sol::variadic_args args) {
            double maxVal = args[0].as<double>();
            for (auto v : args) {
                double val = v.as<double>();
                if (val > maxVal) maxVal = val;
            }
            return maxVal;
        });
        
        // Генерация случайных чисел
        math["random"] = sol::overload(
            []() { return (double)std::rand() / RAND_MAX; },
            [](int m) { return std::rand() % m + 1; },
            [](int m, int n) { return std::rand() % (n - m + 1) + m; }
        );
        
        math["randomseed"] = [](int seed) { std::srand(seed); };
        
        // Дополнительные функции
        math["clamp"] = [](double value, double min, double max) {
            return std::max(min, std::min(max, value));
        };
        
        math["lerp"] = [](double a, double b, double t) {
            return a + (b - a) * t;
        };
        
        math["inverselerp"] = [](double a, double b, double value) {
            return (value - a) / (b - a);
        };
        
        math["remap"] = [](double value, double inMin, double inMax, double outMin, double outMax) {
            double t = (value - inMin) / (inMax - inMin);
            return outMin + (outMax - outMin) * t;
        };
        
        math["sign"] = [](double x) { return x > 0 ? 1 : (x < 0 ? -1 : 0); };
        
        // Векторные операции
        math["vector2"] = lua.create_table();
        math["vector2"]["new"] = [](double x, double y) {
            return sol::as_table(std::vector<double>{x, y});
        };
        
        math["vector2"]["dot"] = [](const std::vector<double>& a, const std::vector<double>& b) {
            return a[0] * b[0] + a[1] * b[1];
        };
        
        math["vector2"]["length"] = [](const std::vector<double>& v) {
            return std::sqrt(v[0] * v[0] + v[1] * v[1]);
        };
        
        math["vector2"]["normalize"] = [](const std::vector<double>& v) {
            double len = std::sqrt(v[0] * v[0] + v[1] * v[1]);
            if (len > 0) {
                return sol::as_table(std::vector<double>{v[0] / len, v[1] / len});
            }
            return sol::as_table(std::vector<double>{0, 0});
        };
        
        math["vector2"]["distance"] = [](const std::vector<double>& a, const std::vector<double>& b) {
            double dx = a[0] - b[0];
            double dy = a[1] - b[1];
            return std::sqrt(dx * dx + dy * dy);
        };
    }

    void ScriptEngine::registerStringLibrary() {
        auto string = lua["string"];
        
        // Базовая функциональность
        string["byte"] = [](const std::string& s, int i) { return (int)s[i-1]; };
        string["char"] = sol::variadic_args([](sol::variadic_args args) {
            std::string result;
            for (auto v : args) {
                result += static_cast<char>(v.as<int>());
            }
            return result;
        });
        
        string["len"] = [](const std::string& s) { return (int)s.length(); };
        string["reverse"] = [](const std::string& s) {
            return std::string(s.rbegin(), s.rend());
        };
        
        string["lower"] = [](const std::string& s) {
            std::string result = s;
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
            return result;
        };
        
        string["upper"] = [](const std::string& s) {
            std::string result = s;
            std::transform(result.begin(), result.end(), result.begin(), ::toupper);
            return result;
        };
        
        string["sub"] = [](const std::string& s, int i, int j) {
            if (i < 1) i = 1;
            if (j == -1) j = (int)s.length();
            if (j < i) return std::string();
            return s.substr(i-1, j-i+1);
        };
        
        // Поиск и замена
        string["find"] = [](const std::string& s, const std::string& pattern, int init, bool plain) {
            if (init < 1) init = 1;
            size_t pos = plain ? s.find(pattern, init-1) : std::string::npos; // TODO: regex
            
            if (pos != std::string::npos) {
                return sol::as_table(std::vector<int>{(int)pos+1, (int)(pos + pattern.length())});
            }
            return sol::nil;
        };
        
        string["gmatch"] = [](const std::string& s, const std::string& pattern) {
            // TODO: реализация gmatch
            return lua.create_table();
        };
        
        string["gsub"] = [](const std::string& s, const std::string& pattern, const std::string& repl, int n) {
            // TODO: реализация gsub с regex
            return std::make_tuple(s, 0);
        };
        
        string["match"] = [](const std::string& s, const std::string& pattern, int init) {
            // TODO: реализация match
            return sol::nil;
        };
        
        // Форматирование
        string["format"] = sol::variadic_args([](sol::variadic_args args) {
            std::string format = args[0].as<std::string>();
            // TODO: полноценное форматирование
            return format;
        });
        
        string["rep"] = [](const std::string& s, int n) {
            std::string result;
            for (int i = 0; i < n; i++) {
                result += s;
            }
            return result;
        };
        
        // Разделение и объединение
        string["split"] = [](const std::string& s, const std::string& delimiter) {
            std::vector<std::string> result;
            size_t start = 0;
            size_t end = s.find(delimiter);
            
            while (end != std::string::npos) {
                result.push_back(s.substr(start, end - start));
                start = end + delimiter.length();
                end = s.find(delimiter, start);
            }
            result.push_back(s.substr(start));
            
            return sol::as_table(result);
        };
        
        string["join"] = [](const sol::table& t, const std::string& delimiter) {
            std::string result;
            for (size_t i = 1; i <= t.size(); i++) {
                if (i > 1) result += delimiter;
                result += t[i].get<std::string>();
            }
            return result;
        };
        
        // Trim
        string["trim"] = [](const std::string& s) {
            auto front = std::find_if_not(s.begin(), s.end(), ::isspace);
            auto back = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
            return (back <= front) ? std::string() : std::string(front, back);
        };
        
        string["ltrim"] = [](const std::string& s) {
            auto front = std::find_if_not(s.begin(), s.end(), ::isspace);
            return std::string(front, s.end());
        };
        
        string["rtrim"] = [](const std::string& s) {
            auto back = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
            return std::string(s.begin(), back);
        };
        
        // Старт/конец
        string["startswith"] = [](const std::string& s, const std::string& prefix) {
            return s.rfind(prefix, 0) == 0;
        };
        
        string["endswith"] = [](const std::string& s, const std::string& suffix) {
            if (suffix.length() > s.length()) return false;
            return s.compare(s.length() - suffix.length(), suffix.length(), suffix) == 0;
        };
        
        // Кодировка
        string["toutf8"] = [](const std::string& s) { return s; };
        string["toutf16"] = [](const std::string& s) {
            // TODO: конвертация в UTF-16
            return std::vector<uint16_t>();
        };
    }

    void ScriptEngine::registerTableLibrary() {
        auto table = lua["table"];
        
        // Базовая функциональность
        table["concat"] = [](const sol::table& t, const std::string& sep, int i, int j) {
            std::string result;
            int start = i ? i : 1;
            int end = j ? j : (int)t.size();
            
            for (int idx = start; idx <= end; idx++) {
                if (idx > start && !sep.empty()) result += sep;
                result += t[idx].get<std::string>();
            }
            return result;
        };
        
        table["insert"] = sol::overload(
            [](sol::table& t, const sol::object& value) {
                t[t.size() + 1] = value;
            },
            [](sol::table& t, int pos, const sol::object& value) {
                // Сдвиг элементов
                for (int i = t.size(); i >= pos; i--) {
                    t[i + 1] = t[i];
                }
                t[pos] = value;
            }
        );
        
        table["remove"] = sol::overload(
            [](sol::table& t) {
                if (t.size() == 0) return sol::nil;
                auto value = t[t.size()];
                t[t.size()] = sol::nil;
                return value;
            },
            [](sol::table& t, int pos) {
                if (pos < 1 || pos > t.size()) return sol::nil;
                auto value = t[pos];
                for (int i = pos; i < t.size(); i++) {
                    t[i] = t[i + 1];
                }
                t[t.size()] = sol::nil;
                return value;
            }
        );
        
        table["sort"] = [](sol::table& t, sol::function comp) {
            // TODO: реализация сортировки с компаратором
            return;
        };
        
        // Дополнительные функции
        table["clear"] = [](sol::table& t) {
            for (int i = 1; i <= t.size(); i++) {
                t[i] = sol::nil;
            }
        };
        
        table["copy"] = [](const sol::table& t) {
            sol::table copy = lua.create_table();
            for (auto& pair : t) {
                copy[pair.first] = pair.second;
            }
            return copy;
        };
        
        table["deepcopy"] = [this](const sol::table& t) {
            return deepCopyTable(t);
        };
        
        table["find"] = [](const sol::table& t, const sol::object& value) {
            for (auto& pair : t) {
                if (pair.second == value) {
                    return pair.first;
                }
            }
            return sol::nil;
        };
        
        table["contains"] = [](const sol::table& t, const sol::object& value) {
            for (auto& pair : t) {
                if (pair.second == value) {
                    return true;
                }
            }
            return false;
        };
        
        table["keys"] = [](const sol::table& t) {
            sol::table keys = lua.create_table();
            int i = 1;
            for (auto& pair : t) {
                keys[i++] = pair.first;
            }
            return keys;
        };
        
        table["values"] = [](const sol::table& t) {
            sol::table values = lua.create_table();
            int i = 1;
            for (auto& pair : t) {
                values[i++] = pair.second;
            }
            return values;
        };
        
        table["merge"] = [](sol::table& t1, const sol::table& t2) {
            for (auto& pair : t2) {
                t1[pair.first] = pair.second;
            }
        };
        
        table["size"] = [](const sol::table& t) {
            return static_cast<int>(t.size());
        };
        
        table["isempty"] = [](const sol::table& t) {
            return t.size() == 0;
        };
        
        // Итераторы
        table["ipairs"] = [](const sol::table& t) {
            return lua.create_table_with(
                "iter", [](const sol::table& t, int i) -> std::tuple<int, sol::object> {
                    i++;
                    auto val = t[i];
                    if (val == sol::nil) {
                        return std::make_tuple(0, sol::nil);
                    }
                    return std::make_tuple(i, val);
                },
                "t", t,
                "i", 0
            );
        };
        
        table["pairs"] = [](const sol::table& t) {
            auto it = t.begin();
            return lua.create_table_with(
                "iter", [it](const sol::table& t) mutable -> std::tuple<sol::object, sol::object> {
                    if (it == t.end()) {
                        return std::make_tuple(sol::nil, sol::nil);
                    }
                    auto pair = *it;
                    ++it;
                    return std::make_tuple(pair.first, pair.second);
                },
                "t", t
            );
        };
    }

    void ScriptEngine::registerOSLibrary() {
        auto os = lua["os"];
        
        if (!sandboxEnabled || hasPermission("os")) {
            // Полный доступ к OS функциям
            os["clock"] = []() { return (double)clock() / CLOCKS_PER_SEC; };
            os["date"] = sol::overload(
                []() { 
                    time_t now = time(nullptr);
                    return ctime(&now);
                },
                [](const std::string& format, time_t time) {
                    char buffer[256];
                    strftime(buffer, sizeof(buffer), format.c_str(), localtime(&time));
                    return std::string(buffer);
                }
            );
            os["difftime"] = [](time_t t2, time_t t1) { return difftime(t2, t1); };
            os["time"] = sol::overload(
                []() { return time(nullptr); },
                [](const sol::table& t) {
                    struct tm tm = {};
                    tm.tm_year = t["year"].get_or(0) - 1900;
                    tm.tm_mon = t["month"].get_or(0) - 1;
                    tm.tm_mday = t["day"].get_or(0);
                    tm.tm_hour = t["hour"].get_or(0);
                    tm.tm_min = t["min"].get_or(0);
                    tm.tm_sec = t["sec"].get_or(0);
                    tm.tm_isdst = t["isdst"].get_or(-1);
                    return mktime(&tm);
                }
            );
            
            os["execute"] = [](const std::string& command) {
                #ifdef _WIN32
                    return system(command.c_str());
                #else
                    return system(command.c_str());
                #endif
            };
            
            os["exit"] = [](int code, bool shouldClose) {
                if (shouldClose) {
                    exit(code);
                }
            };
            
            os["getenv"] = [](const std::string& name) {
                char* value = getenv(name.c_str());
                return value ? std::string(value) : sol::nil;
            };
            
            os["remove"] = [](const std::string& filename) {
                return remove(filename.c_str()) == 0;
            };
            
            os["rename"] = [](const std::string& oldname, const std::string& newname) {
                return rename(oldname.c_str(), newname.c_str()) == 0;
            };
            
            os["tmpname"] = []() {
                #ifdef _WIN32
                    char buffer[MAX_PATH];
                    GetTempFileName(".", "tmp", 0, buffer);
                    return std::string(buffer);
                #else
                    return std::string("/tmp/lua_XXXXXX");
                #endif
            };
        } else {
            // Ограниченный доступ в песочнице
            os["clock"] = []() { return (double)clock() / CLOCKS_PER_SEC; };
            os["date"] = []() { 
                time_t now = time(nullptr);
                return ctime(&now);
            };
            os["difftime"] = [](time_t t2, time_t t1) { return difftime(t2, t1); };
            os["time"] = []() { return time(nullptr); };
            
            // Безопасные функции
            os["getenv"] = [this](const std::string& name) {
                // Только разрешенные переменные
                if (name == "PATH" || name == "TEMP" || name == "TMP") {
                    char* value = getenv(name.c_str());
                    return value ? std::string(value) : sol::nil;
                }
                return sol::nil;
            };
            
            // Запрещенные функции возвращают ошибку
            os["execute"] = [](const std::string&) {
                return -1; // Ошибка: запрещено в песочнице
            };
            
            os["exit"] = [](int, bool) {
                // Игнорируем в песочнице
            };
            
            os["remove"] = [](const std::string&) { return false; };
            os["rename"] = [](const std::string&, const std::string&) { return false; };
            os["tmpname"] = []() { return std::string(""); };
        }
    }

    void ScriptEngine::registerIOLibrary() {
        auto io = lua["io"];
        
        if (!sandboxEnabled || hasPermission("io")) {
            // Полный доступ к IO
            io["open"] = [](const std::string& filename, const std::string& mode) {
                FILE* file = fopen(filename.c_str(), mode.c_str());
                if (!file) return sol::nil;
                
                // Создаем объект файла
                return lua.create_table_with(
                    "close", [file]() { fclose(file); },
                    "read", [file](sol::variadic_args args) {
                        // TODO: реализация read
                        return sol::nil;
                    },
                    "write", [file](const std::string& str) {
                        fwrite(str.c_str(), 1, str.length(), file);
                        return true;
                    },
                    "flush", [file]() { fflush(file); },
                    "seek", [file](const std::string& whence, int offset) {
                        int origin = SEEK_SET;
                        if (whence == "cur") origin = SEEK_CUR;
                        else if (whence == "end") origin = SEEK_END;
                        fseek(file, offset, origin);
                        return ftell(file);
                    }
                );
            };
            
            io["input"] = sol::overload(
                []() { return stdin; },
                [](const std::string& filename) {
                    return freopen(filename.c_str(), "r", stdin);
                }
            );
            
            io["output"] = sol::overload(
                []() { return stdout; },
                [](const std::string& filename) {
                    return freopen(filename.c_str(), "w", stdout);
                }
            );
            
            io["read"] = sol::variadic_args([](sol::variadic_args args) {
                // TODO: реализация read
                return sol::nil;
            });
            
            io["write"] = sol::variadic_args([](sol::variadic_args args) {
                for (auto v : args) {
                    std::cout << v.as<std::string>();
                }
                return true;
            });
            
            io["flush"] = []() { fflush(stdout); };
            
            io["lines"] = [](const std::string& filename) {
                // TODO: реализация lines
                return lua.create_table();
            };
            
            io["type"] = [](sol::object obj) {
                if (obj.is<FILE*>()) {
                    FILE* file = obj.as<FILE*>();
                    if (ferror(file)) return "closed file";
                    return "file";
                }
                return "nil";
            };
            
            io["tmpfile"] = []() {
                return tmpfile();
            };
            
            io["close"] = [](FILE* file) {
                if (file && file != stdin && file != stdout && file != stderr) {
                    fclose(file);
                }
            };
        } else {
            // Ограниченный доступ в песочнице
            io["open"] = [this](const std::string& filename, const std::string& mode) {
                // Проверяем разрешенные пути
                if (!isPathAllowed(filename)) {
                    SPDLOG_WARN("File access denied in sandbox: {}", filename);
                    return sol::nil;
                }
                
                FILE* file = fopen(filename.c_str(), mode.c_str());
                if (!file) return sol::nil;
                
                // Создаем объект файла с ограниченными методами
                return lua.create_table_with(
                    "close", [file]() { fclose(file); },
                    "read", [](sol::variadic_args) { return sol::nil; },
                    "write", [](const std::string&) { return false; }
                );
            };
            
            // Только чтение
            io["read"] = sol::variadic_args([](sol::variadic_args) { return sol::nil; });
            io["write"] = sol::variadic_args([](sol::variadic_args) { return false; });
            io["flush"] = []() { };
            
            // Запрещенные функции
            io["input"] = []() { return sol::nil; };
            io["output"] = []() { return sol::nil; };
            io["lines"] = [](const std::string&) { return sol::nil; };
            io["tmpfile"] = []() { return sol::nil; };
            io["close"] = [](sol::object) { };
            io["type"] = [](sol::object) { return "nil"; };
        }
        
        // Стандартные потоки
        io["stdin"] = stdin;
        io["stdout"] = stdout;
        io["stderr"] = stderr;
    }

    void ScriptEngine::registerDebugLibrary() {
        auto debug = lua["debug"];
        
        if (debuggerEnabled) {
            // Полный доступ к отладке
            debug["debug"] = []() {
                // TODO: запуск отладчика
                return;
            };
            
            debug["gethook"] = [](sol::thread thread) {
                // TODO: получение информации о хуках
                return sol::nil;
            };
            
            debug["getinfo"] = [](int level, const std::string& what) {
                // TODO: получение информации о стеке вызовов
                return lua.create_table();
            };
            
            debug["getlocal"] = [](int level, int local) {
                // TODO: получение локальной переменной
                return std::make_tuple("", sol::nil);
            };
            
            debug["getregistry"] = []() {
                return sol::registry;
            };
            
            debug["getupvalue"] = [](sol::function func, int up) {
                // TODO: получение upvalue
                return std::make_tuple("", sol::nil);
            };
            
            debug["getuservalue"] = [](sol::object u) {
                // TODO: получение user value
                return sol::nil;
            };
            
            debug["sethook"] = [](sol::thread thread, sol::function hook, const std::string& mask, int count) {
                // TODO: установка хука
                return;
            };
            
            debug["setlocal"] = [](int level, int local, sol::object value) {
                // TODO: установка локальной переменной
                return "";
            };
            
            debug["setupvalue"] = [](sol::function func, int up, sol::object value) {
                // TODO: установка upvalue
                return "";
            };
            
            debug["setuservalue"] = [](sol::object u, sol::object value) {
                // TODO: установка user value
                return;
            };
            
            debug["traceback"] = [](sol::thread thread, const std::string& message, int level) {
                // TODO: генерация traceback
                return message;
            };
            
            debug["upvalueid"] = [](sol::function func, int n) {
                // TODO: получение id upvalue
                return sol::lightuserdata{nullptr};
            };
            
            debug["upvaluejoin"] = [](sol::function func1, int n1, sol::function func2, int n2) {
                // TODO: объединение upvalues
                return;
            };
        } else {
            // Ограниченный доступ к отладке
            debug["traceback"] = [](sol::thread thread, const std::string& message, int level) {
                return message + "\n[Debugging disabled]";
            };
            
            // Запрещенные функции
            debug["debug"] = []() { };
            debug["gethook"] = [](sol::thread) { return sol::nil; };
            debug["getinfo"] = [](int, const std::string&) { return sol::nil; };
            debug["getlocal"] = [](int, int) { return std::make_tuple("", sol::nil); };
            debug["getregistry"] = []() { return sol::nil; };
            debug["getupvalue"] = [](sol::function, int) { return std::make_tuple("", sol::nil); };
            debug["getuservalue"] = [](sol::object) { return sol::nil; };
            debug["sethook"] = [](sol::thread, sol::function, const std::string&, int) { };
            debug["setlocal"] = [](int, int, sol::object) { return ""; };
            debug["setupvalue"] = [](sol::function, int, sol::object) { return ""; };
            debug["setuservalue"] = [](sol::object, sol::object) { };
            debug["upvalueid"] = [](sol::function, int) { return sol::lightuserdata{nullptr}; };
            debug["upvaluejoin"] = [](sol::function, int, sol::function, int) { };
        }
    }

    void ScriptEngine::registerCoroutineLibrary() {
        auto coroutine = lua["coroutine"];
        
        coroutine["create"] = [this](sol::function func) {
            sol::thread thread = lua.create_thread(func);
            coroutines.push_back(thread);
            return thread;
        };
        
        coroutine["resume"] = [](sol::thread thread, sol::variadic_args args) {
            std::vector<sol::object> argList;
            for (auto v : args) {
                argList.push_back(v);
            }
            
            sol::protected_function_result result;
            if (argList.empty()) {
                result = thread();
            } else {
                result = thread(sol::as_args(argList));
            }
            
            if (!result.valid()) {
                sol::error err = result;
                return std::make_tuple(false, err.what());
            }
            
            std::vector<sol::object> returnValues;
            for (int i = 0; i < result.return_count(); i++) {
                returnValues.push_back(result[i]);
            }
            
            if (returnValues.empty()) {
                return std::make_tuple(true);
            }
            
            return std::make_tuple(true, sol::as_args(returnValues));
        };
        
        coroutine["running"] = []() {
            // TODO: получение текущей корутины
            return std::make_tuple(sol::nil, false);
        };
        
        coroutine["status"] = [](sol::thread thread) {
            if (thread.state() == sol::thread_status::dead) {
                return "dead";
            } else if (thread.state() == sol::thread_status::yielded) {
                return "suspended";
            } else if (thread.state() == sol::thread_status::ok) {
                return "running";
            }
            return "normal";
        };
        
        coroutine["wrap"] = [this](sol::function func) {
            sol::thread thread = lua.create_thread(func);
            coroutines.push_back(thread);
            
            return [thread](sol::variadic_args args) mutable {
                std::vector<sol::object> argList;
                for (auto v : args) {
                    argList.push_back(v);
                }
                
                sol::protected_function_result result;
                if (argList.empty()) {
                    result = thread();
                } else {
                    result = thread(sol::as_args(argList));
                }
                
                if (!result.valid()) {
                    sol::error err = result;
                    throw err;
                }
                
                std::vector<sol::object> returnValues;
                for (int i = 0; i < result.return_count(); i++) {
                    returnValues.push_back(result[i]);
                }
                
                if (returnValues.empty()) {
                    return;
                }
                
                if (returnValues.size() == 1) {
                    return returnValues[0];
                }
                
                return sol::as_returns(returnValues);
            };
        };
        
        coroutine["yield"] = sol::variadic_args([](sol::variadic_args args) {
            // TODO: реализация yield
            return sol::nil;
        });
        
        // Дополнительные функции
        coroutine["wait"] = [this](float seconds) {
            return createWaitCoroutine(seconds);
        };
        
        coroutine["waitframes"] = [this](int frames) {
            return createWaitFramesCoroutine(frames);
        };
        
        coroutine["waituntill"] = [this](sol::function condition) {
            return createWaitUntilCoroutine(condition);
        };
        
        coroutine["waitwhile"] = [this](sol::function condition) {
            return createWaitWhileCoroutine(condition);
        };
    }

    void ScriptEngine::registerEngineAPI() {
        SPDLOG_INFO("Registering engine API...");
        
        // Регистрация основных типов
        registerVectorTypes();
        registerColorType();
        registerRectType();
        registerTransformType();
        registerGameObjectType();
        registerComponentType();
        registerSceneType();
        
        // Регистрация системных API
        registerTimeAPI();
        registerInputAPI();
        registerGraphicsAPI();
        registerAudioAPI();
        registerPhysicsAPI();
        registerUIManagerAPI();
        registerNetworkAPI();
        registerEventAPI();
        registerResourceAPI();
        registerDebugAPI();
        registerMathAPI();
        registerRandomAPI();
        registerFileSystemAPI();
        registerJSONAPI();
        registerPrefabAPI();
        registerSaveSystemAPI();
        registerLocalizationAPI();
        
        // Регистрация компонентов
        registerSpriteRendererAPI();
        registerCameraAPI();
        registerRigidbody2DAPI();
        registerColliderAPI();
        registerAudioSourceAPI();
        registerParticleSystemAPI();
        registerAnimationAPI();
        registerLightAPI();
        registerTerrainAPI();
        registerWaterAPI();
        registerNavMeshAgentAPI();
        
        // Регистрация системы событий
        registerEventSystemAPI();
        
        // Регистрация AI системы
        registerAISystemAPI();
        
        // Регистрация системы квестов
        registerQuestSystemAPI();
        
        // Регистрация инвентаря
        registerInventoryAPI();
        
        // Регистрация диалоговой системы
        registerDialogSystemAPI();
        
        // Регистрация системы модов
        registerModdingAPI();
        
        // Регистрация Steam API (если доступно)
        #ifdef WITH_STEAM
        registerSteamAPI();
        #endif
        
        // Регистрация Discord API (если доступно)
        #ifdef WITH_DISCORD
        registerDiscordAPI();
        #endif
        
        SPDLOG_INFO("Engine API registered successfully");
    }

    void ScriptEngine::registerTimeAPI() {
        auto timeTable = lua.create_table("Time");
        
        // Статические свойства
        timeTable["deltaTime"] = []() -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getDeltaTime() : 0.0f;
        };
        
        timeTable["fixedDeltaTime"] = []() -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getFixedDeltaTime() : 0.016f;
        };
        
        timeTable["time"] = []() -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getTime() : 0.0f;
        };
        
        timeTable["realtimeSinceStartup"] = []() -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getRealtimeSinceStartup() : 0.0f;
        };
        
        timeTable["timeScale"] = sol::property(
            []() -> float {
                auto& engine = Engine::getInstance();
                auto timeManager = engine.getTimeManager();
                return timeManager ? timeManager->getTimeScale() : 1.0f;
            },
            [](float scale) {
                auto& engine = Engine::getInstance();
                auto timeManager = engine.getTimeManager();
                if (timeManager) {
                    timeManager->setTimeScale(scale);
                }
            }
        );
        
        timeTable["frameCount"] = []() -> int {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getFrameCount() : 0;
        };
        
        timeTable["unscaledTime"] = []() -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getUnscaledTime() : 0.0f;
        };
        
        timeTable["unscaledDeltaTime"] = []() -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getUnscaledDeltaTime() : 0.0f;
        };
        
        // Методы
        timeTable["WaitForSeconds"] = [this](float seconds) -> sol::coroutine {
            return createWaitCoroutine(seconds);
        };
        
        timeTable["WaitForNextFrame"] = [this]() -> sol::coroutine {
            return createWaitFramesCoroutine(1);
        };
        
        timeTable["WaitForEndOfFrame"] = [this]() -> sol::coroutine {
            return createWaitEndOfFrameCoroutine();
        };
        
        timeTable["WaitUntil"] = [this](sol::function condition) -> sol::coroutine {
            return createWaitUntilCoroutine(condition);
        };
        
        timeTable["WaitWhile"] = [this](sol::function condition) -> sol::coroutine {
            return createWaitWhileCoroutine(condition);
        };
        
        timeTable["CreateTimer"] = [](float duration, sol::function callback, bool repeating = false) -> std::string {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            if (!timeManager) return "";
            
            std::string timerId = "timer_" + std::to_string(std::rand());
            timeManager->startTimer(timerId, duration, [callback]() {
                try {
                    callback();
                } catch (const sol::error& e) {
                    SPDLOG_ERROR("Timer callback error: {}", e.what());
                }
            }, repeating);
            
            return timerId;
        };
        
        timeTable["StopTimer"] = [](const std::string& timerId) {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            if (timeManager) {
                timeManager->stopTimer(timerId);
            }
        };
        
        timeTable["PauseTimer"] = [](const std::string& timerId) {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            if (timeManager) {
                timeManager->pauseTimer(timerId);
            }
        };
        
        timeTable["ResumeTimer"] = [](const std::string& timerId) {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            if (timeManager) {
                timeManager->resumeTimer(timerId);
            }
        };
        
        timeTable["IsTimerRunning"] = [](const std::string& timerId) -> bool {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->isTimerRunning(timerId) : false;
        };
        
        timeTable["GetTimerElapsed"] = [](const std::string& timerId) -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getTimerElapsed(timerId) : 0.0f;
        };
        
        timeTable["GetTimerRemaining"] = [](const std::string& timerId) -> float {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            return timeManager ? timeManager->getTimerRemaining(timerId) : 0.0f;
        };
    }

    void ScriptEngine::registerInputAPI() {
        auto inputTable = lua.create_table("Input");
        
        // Клавиатура
        inputTable["GetKey"] = [](const std::string& keyName) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getKey(keyName) : false;
        };
        
        inputTable["GetKeyDown"] = [](const std::string& keyName) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getKeyDown(keyName) : false;
        };
        
        inputTable["GetKeyUp"] = [](const std::string& keyName) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getKeyUp(keyName) : false;
        };
        
        // Мышь
        inputTable["GetMouseButton"] = [](int button) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getMouseButton(button) : false;
        };
        
        inputTable["GetMouseButtonDown"] = [](int button) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getMouseButtonDown(button) : false;
        };
        
        inputTable["GetMouseButtonUp"] = [](int button) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getMouseButtonUp(button) : false;
        };
        
        inputTable["GetMousePosition"] = []() -> sol::table {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            if (!input) return lua.create_table();
            
            Vector2 pos = input->getMousePosition();
            return lua.create_table_with("x", pos.x, "y", pos.y);
        };
        
        inputTable["GetMouseScrollDelta"] = []() -> sol::table {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            if (!input) return lua.create_table();
            
            Vector2 delta = input->getMouseScrollDelta();
            return lua.create_table_with("x", delta.x, "y", delta.y);
        };
        
        // Оси
        inputTable["GetAxis"] = [](const std::string& axisName) -> float {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getAxis(axisName) : 0.0f;
        };
        
        inputTable["GetAxisRaw"] = [](const std::string& axisName) -> float {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getAxisRaw(axisName) : 0.0f;
        };
        
        // Геймпад
        inputTable["GetGamepadButton"] = [](int gamepad, const std::string& button) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getGamepadButton(gamepad, button) : false;
        };
        
        inputTable["GetGamepadAxis"] = [](int gamepad, const std::string& axis) -> float {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getGamepadAxis(gamepad, axis) : 0.0f;
        };
        
        inputTable["IsGamepadConnected"] = [](int gamepad) -> bool {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->isGamepadConnected(gamepad) : false;
        };
        
        // Тачскрин
        inputTable["GetTouch"] = [](int fingerId) -> sol::table {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            if (!input) return lua.create_table();
            
            Touch touch = input->getTouch(fingerId);
            return lua.create_table_with(
                "fingerId", touch.fingerId,
                "position", lua.create_table_with("x", touch.position.x, "y", touch.position.y),
                "delta", lua.create_table_with("x", touch.delta.x, "y", touch.delta.y),
                "pressure", touch.pressure,
                "phase", static_cast<int>(touch.phase)
            );
        };
        
        inputTable["GetTouchCount"] = []() -> int {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            return input ? input->getTouchCount() : 0;
        };
        
        // Графический планшет
        inputTable["GetTabletPressure"] = []() -> float {
            auto& engine = Engine::getInstance();
            auto tablet = engine.getGraphicsTablet();
            return (tablet && tablet->isConnected()) ? tablet->getData().pressure : 0.0f;
        };
        
        inputTable["IsTabletConnected"] = []() -> bool {
            auto& engine = Engine::getInstance();
            auto tablet = engine.getGraphicsTablet();
            return tablet && tablet->isConnected();
        };
        
        // Управление курсором
        inputTable["SetCursorLocked"] = [](bool locked) {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            if (input) {
                input->setCursorLocked(locked);
            }
        };
        
        inputTable["SetCursorVisible"] = [](bool visible) {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            if (input) {
                input->setCursorVisible(visible);
            }
        };
        
        inputTable["SetCursorPosition"] = [](int x, int y) {
            auto& engine = Engine::getInstance();
            auto input = engine.getInput();
            if (input) {
                input->setCursorPosition(x, y);
            }
        };
    }

    void ScriptEngine::registerGraphicsAPI() {
        auto graphicsTable = lua.create_table("Graphics");
        
        // Рендеринг
        graphicsTable["DrawLine"] = [](float x1, float y1, float x2, float y2, sol::table color) {
            auto& engine = Engine::getInstance();
            auto debugDraw = engine.getDebugDraw();
            if (debugDraw) {
                Color col(
                    color["r"].get_or(1.0f),
                    color["g"].get_or(1.0f),
                    color["b"].get_or(1.0f),
                    color["a"].get_or(1.0f)
                );
                debugDraw->drawLine(Vector2(x1, y1), Vector2(x2, y2), col);
            }
        };
        
        graphicsTable["DrawRect"] = [](float x, float y, float width, float height, sol::table color, bool filled = true) {
            auto& engine = Engine::getInstance();
            auto debugDraw = engine.getDebugDraw();
            if (debugDraw) {
                Color col(
                    color["r"].get_or(1.0f),
                    color["g"].get_or(1.0f),
                    color["b"].get_or(1.0f),
                    color["a"].get_or(1.0f)
                );
                debugDraw->drawRect(Rect(x, y, width, height), col, filled);
            }
        };
        
        graphicsTable["DrawCircle"] = [](float x, float y, float radius, sol::table color, bool filled = true) {
            auto& engine = Engine::getInstance();
            auto debugDraw = engine.getDebugDraw();
            if (debugDraw) {
                Color col(
                    color["r"].get_or(1.0f),
                    color["g"].get_or(1.0f),
                    color["b"].get_or(1.0f),
                    color["a"].get_or(1.0f)
                );
                debugDraw->drawCircle(Vector2(x, y), radius, col, filled);
            }
        };
        
        graphicsTable["DrawText"] = [](const std::string& text, float x, float y, sol::table color, float size = 16.0f) {
            auto& engine = Engine::getInstance();
            auto debugDraw = engine.getDebugDraw();
            if (debugDraw) {
                Color col(
                    color["r"].get_or(1.0f),
                    color["g"].get_or(1.0f),
                    color["b"].get_or(1.0f),
                    color["a"].get_or(1.0f)
                );
                debugDraw->drawText(text, Vector2(x, y), col, size);
            }
        };
        
        // Экран
        graphicsTable["GetScreenWidth"] = []() -> int {
            auto& engine = Engine::getInstance();
            return engine.getWindowWidth();
        };
        
        graphicsTable["GetScreenHeight"] = []() -> int {
            auto& engine = Engine::getInstance();
            return engine.getWindowHeight();
        };
        
        graphicsTable["GetScreenSize"] = []() -> sol::table {
            auto& engine = Engine::getInstance();
            return lua.create_table_with(
                "width", engine.getWindowWidth(),
                "height", engine.getWindowHeight()
            );
        };
        
        graphicsTable["ScreenToWorldPoint"] = [](float screenX, float screenY) -> sol::table {
            auto& engine = Engine::getInstance();
            auto camera = engine.getMainCamera();
            if (camera) {
                Vector2 worldPos = camera->screenToWorld(Vector2(screenX, screenY));
                return lua.create_table_with("x", worldPos.x, "y", worldPos.y);
            }
            return lua.create_table_with("x", screenX, "y", screenY);
        };
        
        graphicsTable["WorldToScreenPoint"] = [](float worldX, float worldY) -> sol::table {
            auto& engine = Engine::getInstance();
            auto camera = engine.getMainCamera();
            if (camera) {
                Vector2 screenPos = camera->worldToScreen(Vector2(worldX, worldY));
                return lua.create_table_with("x", screenPos.x, "y", screenPos.y);
            }
            return lua.create_table_with("x", worldX, "y", worldY);
        };
        
        // Камера
        graphicsTable["GetMainCamera"] = []() -> Camera* {
            auto& engine = Engine::getInstance();
            return engine.getMainCamera();
        };
        
        graphicsTable["SetCameraPosition"] = [](float x, float y) {
            auto& engine = Engine::getInstance();
            auto camera = engine.getMainCamera();
            if (camera) {
                camera->setPosition(Vector2(x, y));
            }
        };
        
        graphicsTable["SetCameraZoom"] = [](float zoom) {
            auto& engine = Engine::getInstance();
            auto camera = engine.getMainCamera();
            if (camera) {
                camera->setZoom(zoom);
            }
        };
        
        // Эффекты
        graphicsTable["SetScreenShake"] = [](float intensity, float duration) {
            auto& engine = Engine::getInstance();
            auto camera = engine.getMainCamera();
            if (camera) {
                camera->setScreenShake(intensity, duration);
            }
        };
        
        graphicsTable["SetSlowMotion"] = [](float timeScale, float duration) {
            auto& engine = Engine::getInstance();
            auto timeManager = engine.getTimeManager();
            if (timeManager) {
                timeManager->setTimeScale(timeScale);
                
                // Восстановление через duration секунд
                if (duration > 0) {
                    auto timerId = "slowmo_reset_" + std::to_string(std::rand());
                    timeManager->startTimer(timerId, duration, [timeManager]() {
                        timeManager->setTimeScale(1.0f);
                    });
                }
            }
        };
        
        // Захват экрана
        graphicsTable["CaptureScreenshot"] = [](const std::string& filename = "") {
            auto& engine = Engine::getInstance();
            engine.takeScreenshot(filename);
        };
    }

    std::shared_ptr<Script> ScriptEngine::createScript(const std::string& name) {
        try {
            // Проверка реестра C++ скриптов
            auto it = scriptRegistry.find(name);
            if (it != scriptRegistry.end()) {
                auto script = it->second();
                scriptInstances.push_back(script);
                return script;
            }
            
            // Попытка загрузить как Lua скрипт
            std::string luaName = name;
            if (luaName.find('.') == std::string::npos) {
                luaName = name + ".lua";
            }
            
            if (loadScript(luaName)) {
                // Создание экземпляра скрипта
                sol::table scriptClass = lua[name];
                if (scriptClass.valid()) {
                    sol::object scriptInstance = scriptClass();
                    if (scriptInstance.is<sol::table>()) {
                        auto luaScript = std::make_shared<LuaScript>(scriptInstance);
                        scriptInstances.push_back(luaScript);
                        return luaScript;
                    }
                }
            }
            
            SPDLOG_WARN("Script not found: {}", name);
            return nullptr;
            
        } catch (const sol::error& e) {
            SPDLOG_ERROR("Failed to create script {}: {}", name, e.what());
            return nullptr;
        }
    }

    bool ScriptEngine::loadScript(const std::string& path) {
        try {
            if (!fs::exists(path)) {
                // Попытка найти по package.path
                std::string resolvedPath = resolveScriptPath(path);
                if (resolvedPath.empty()) {
                    SPDLOG_ERROR("Script file not found: {}", path);
                    return false;
                }
                
                return loadScriptFromFile(resolvedPath);
            }
            
            return loadScriptFromFile(path);
            
        } catch (const sol::error& e) {
            SPDLOG_ERROR("Failed to load script {}: {}", path, e.what());
            return false;
        }
    }

    bool ScriptEngine::loadScriptFromFile(const std::string& path) {
        try {
            std::ifstream file(path);
            if (!file.is_open()) {
                SPDLOG_ERROR("Cannot open script file: {}", path);
                return false;
            }
            
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string code = buffer.str();
            
            // Добавляем путь к файлу для отладки
            code = "-- File: " + path + "\n" + code;
            
            // Загружаем скрипт
            sol::protected_function_result result = lua.safe_script(code, sol::script_pass_on_error);
            if (!result.valid()) {
                sol::error err = result;
                SPDLOG_ERROR("Script compilation error in {}: {}", path, err.what());
                return false;
            }
            
            // Сохраняем время модификации для hot-reload
            scriptModificationTimes[path] = fs::last_write_time(path);
            
            // Извлекаем имя модуля из пути
            std::string moduleName = fs::path(path).stem().string();
            loadedPackages[moduleName] = result;
            
            SPDLOG_DEBUG("Script loaded: {}", path);
            return true;
            
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Error loading script {}: {}", path, e.what());
            return false;
        }
    }

    bool ScriptEngine::loadScriptFromString(const std::string& code) {
        try {
            sol::protected_function_result result = lua.safe_script(code, sol::script_pass_on_error);
            if (!result.valid()) {
                sol::error err = result;
                SPDLOG_ERROR("Script compilation error: {}", err.what());
                return false;
            }
            return true;
        } catch (const sol::error& e) {
            SPDLOG_ERROR("Failed to load script from string: {}", e.what());
            return false;
        }
    }

    void ScriptEngine::reloadScripts() {
        SPDLOG_INFO("Reloading modified scripts...");
        
        for (const auto& [path, modTime] : scriptModificationTimes) {
            if (fs::exists(path)) {
                auto currentModTime = fs::last_write_time(path);
                if (currentModTime != modTime) {
                    SPDLOG_INFO("Reloading script: {}", path);
                    loadScriptFromFile(path);
                    scriptModificationTimes[path] = currentModTime;
                }
            }
        }
        
        // Обновление всех экземпляров скриптов
        for (auto& script : scriptInstances) {
            if (auto luaScript = std::dynamic_pointer_cast<LuaScript>(script)) {
                luaScript->onHotReload();
            }
        }
    }

    void ScriptEngine::enableDebugging(bool enable) {
        debuggerEnabled = enable;
        if (enable) {
            initializeDebugger();
        } else {
            shutdownDebugger();
        }
    }

    void ScriptEngine::setBreakpoint(const std::string& file, int line) {
        if (debuggerEnabled) {
            breakpoints[file].insert(line);
            SPDLOG_DEBUG("Breakpoint set: {}:{}", file, line);
        }
    }

    void ScriptEngine::clearBreakpoint(const std::string& file, int line) {
        if (debuggerEnabled) {
            auto it = breakpoints.find(file);
            if (it != breakpoints.end()) {
                it->second.erase(line);
                if (it->second.empty()) {
                    breakpoints.erase(it);
                }
            }
        }
    }

    void ScriptEngine::startProfiling() {
        profilingEnabled = true;
        profileStartTime = std::chrono::high_resolution_clock::now();
        profileData.clear();
        
        SPDLOG_INFO("Script profiling started");
    }

    void ScriptEngine::stopProfiling() {
        profilingEnabled = false;
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - profileStartTime);
        
        SPDLOG_INFO("Script profiling stopped. Duration: {}ms", duration.count());
    }

    Json::Value ScriptEngine::getProfileData() const {
        return profileData;
    }

    void ScriptEngine::collectGarbage() {
        lua.collect_garbage();
    }

    size_t ScriptEngine::getMemoryUsage() const {
        // TODO: более точное измерение памяти Lua
        return lua.memory_used();
    }

    void ScriptEngine::loadPackage(const std::string& packagePath) {
        try {
            if (loadedPackages.find(packagePath) != loadedPackages.end()) {
                return; // Уже загружен
            }
            
            std::string code = loadPackageCode(packagePath);
            if (!code.empty()) {
                sol::protected_function_result result = lua.safe_script(code, sol::script_pass_on_error);
                if (result.valid()) {
                    loadedPackages[packagePath] = result;
                    SPDLOG_DEBUG("Package loaded: {}", packagePath);
                } else {
                    sol::error err = result;
                    SPDLOG_ERROR("Failed to load package {}: {}", packagePath, err.what());
                }
            }
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Error loading package {}: {}", packagePath, e.what());
        }
    }

    void ScriptEngine::unloadPackage(const std::string& packageName) {
        auto it = loadedPackages.find(packageName);
        if (it != loadedPackages.end()) {
            loadedPackages.erase(it);
            SPDLOG_DEBUG("Package unloaded: {}", packageName);
        }
    }

    void ScriptEngine::enableSandbox(bool enable) {
        sandboxEnabled = enable;
        if (enable) {
            setupSandbox();
        } else {
            removeSandbox();
        }
    }

    void ScriptEngine::setSandboxPermissions(const std::vector<std::string>& permissions) {
        sandboxPermissions.clear();
        for (const auto& perm : permissions) {
            sandboxPermissions.insert(perm);
        }
    }

    void ScriptEngine::createThread(const std::string& code) {
        try {
            auto threadFunc = [this, code]() {
                try {
                    sol::protected_function_result result = lua.safe_script(code, sol::script_pass_on_error);
                    if (!result.valid()) {
                        sol::error err = result;
                        SPDLOG_ERROR("Thread execution error: {}", err.what());
                    }
                } catch (const std::exception& e) {
                    SPDLOG_ERROR("Thread exception: {}", e.what());
                }
            };
            
            std::thread thread(threadFunc);
            std::lock_guard<std::mutex> lock(threadMutex);
            scriptThreads.push_back(std::move(thread));
            
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Failed to create thread: {}", e.what());
        }
    }

    void ScriptEngine::joinAllThreads() {
        std::lock_guard<std::mutex> lock(threadMutex);
        for (auto& thread : scriptThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        scriptThreads.clear();
    }

    sol::coroutine ScriptEngine::createCoroutine(const std::string& code) {
        try {
            sol::function func = lua.load(code);
            sol::coroutine coroutine = lua.create_coroutine(func);
            coroutines.push_back(coroutine);
            return coroutine;
        } catch (const sol::error& e) {
            SPDLOG_ERROR("Failed to create coroutine: {}", e.what());
            return sol::coroutine();
        }
    }

    void ScriptEngine::resumeCoroutine(sol::coroutine& coroutine) {
        try {
            sol::protected_function_result result = coroutine();
            if (!result.valid()) {
                sol::error err = result;
                SPDLOG_ERROR("Coroutine execution error: {}", err.what());
            }
        } catch (const sol::error& e) {
            SPDLOG_ERROR("Coroutine error: {}", e.what());
        }
    }

    void ScriptEngine::enableJIT(bool enable) {
        #ifdef LUAJIT_VERSION
        jitEnabled = enable;
        if (enable) {
            lua["jit"]["on"]();
        } else {
            lua["jit"]["off"]();
        }
        #endif
    }

    void ScriptEngine::optimizeScript(const std::string& path) {
        // TODO: оптимизация Lua кода
        SPDLOG_INFO("Optimizing script: {}", path);
    }

    void ScriptEngine::generateDocumentation(const std::string& outputPath) {
        // TODO: генерация документации из скриптов
        SPDLOG_INFO("Generating documentation to: {}", outputPath);
    }

    void ScriptEngine::runTests(const std::string& testPath) {
        // TODO: запуск тестов
        SPDLOG_INFO("Running tests from: {}", testPath);
    }

    void ScriptEngine::connectToIDE(const std::string& host, int port) {
        // TODO: подключение к IDE для отладки
        ideConnected = true;
        ideHost = host;
        idePort = port;
        
        SPDLOG_INFO("Connected to IDE: {}:{}", host, port);
    }

    void ScriptEngine::disconnectFromIDE() {
        // TODO: отключение от IDE
        ideConnected = false;
        
        SPDLOG_INFO("Disconnected from IDE");
    }

    // Приватные методы

    void ScriptEngine::setupSandbox() {
        // Создание безопасного окружения
        sol::table sandboxEnv = lua.create_table();
        
        // Копируем безопасные библиотеки
        sandboxEnv["_G"] = sandboxEnv;
        sandboxEnv["math"] = lua["math"];
        sandboxEnv["string"] = lua["string"];
        sandboxEnv["table"] = lua["table"];
        sandboxEnv["coroutine"] = lua["coroutine"];
        
        // Ограниченные библиотеки
        sol::table safeOS = lua.create_table();
        safeOS["clock"] = lua["os"]["clock"];
        safeOS["date"] = lua["os"]["date"];
        safeOS["difftime"] = lua["os"]["difftime"];
        safeOS["time"] = lua["os"]["time"];
        sandboxEnv["os"] = safeOS;
        
        sol::table safeIO = lua.create_table();
        safeIO["type"] = lua["io"]["type"];
        sandboxEnv["io"] = safeIO;
        
        // API движка
        sandboxEnv["Time"] = lua["Time"];
        sandboxEnv["Input"] = lua["Input"];
        sandboxEnv["Graphics"] = lua["Graphics"];
        // ... другие безопасные API
        
        // Устанавливаем окружение для новых потоков
        lua["_SANDBOX_ENV"] = sandboxEnv;
        
        // Метатаблица для ограничения доступа
        sol::table metatable = lua.create_table();
        metatable[sol::meta_function::index] = sandboxEnv;
        metatable[sol::meta_function::new_index] = [](sol::table, sol::object, sol::object) {
            // Запрещаем создание новых глобальных переменных
            SPDLOG_WARN("Attempt to create global variable in sandbox");
        };
        
        sandboxEnv[sol::metatable_key] = metatable;
        
        SPDLOG_INFO("Sandbox environment created");
    }

    void ScriptEngine::removeSandbox() {
        lua["_SANDBOX_ENV"] = sol::nil;
        SPDLOG_INFO("Sandbox removed");
    }

    bool ScriptEngine::hasPermission(const std::string& permission) const {
        return sandboxPermissions.find(permission) != sandboxPermissions.end() ||
               sandboxPermissions.find("*") != sandboxPermissions.end();
    }

    bool ScriptEngine::isPathAllowed(const std::string& path) const {
        if (!sandboxEnabled) return true;
        
        // Разрешаем только относительные пути в пределах проекта
        fs::path filePath(path);
        if (filePath.is_absolute()) {
            return false;
        }
        
        // Проверяем, не выходит ли путь за пределы проекта
        fs::path canonicalPath = fs::canonical(fs::current_path() / filePath);
        fs::path projectRoot = fs::current_path();
        
        // Проверяем, что канонический путь начинается с projectRoot
        auto relPath = fs::relative(canonicalPath, projectRoot);
        return !relPath.empty() && relPath.native().find("..") == std::string::npos;
    }

    void ScriptEngine::initializeDebugger() {
        // TODO: инициализация отладчика Lua
        SPDLOG_INFO("Lua debugger initialized");
    }

    void ScriptEngine::shutdownDebugger() {
        // TODO: завершение работы отладчика
        SPDLOG_INFO("Lua debugger shutdown");
    }

    void ScriptEngine::updateDebugger() {
        // TODO: обновление отладчика
    }

    void ScriptEngine::updateCoroutines(float deltaTime) {
        // Обновление корутин ожидания
        auto it = waitingCoroutines.begin();
        while (it != waitingCoroutines.end()) {
            auto& [coroutine, timer] = *it;
            timer -= deltaTime;
            
            if (timer <= 0) {
                resumeCoroutine(coroutine);
                it = waitingCoroutines.erase(it);
            } else {
                ++it;
            }
        }
        
        // Обновление корутин ожидания кадров
        auto frameIt = frameWaitingCoroutines.begin();
        while (frameIt != frameWaitingCoroutines.end()) {
            auto& [coroutine, frames] = *frameIt;
            frames--;
            
            if (frames <= 0) {
                resumeCoroutine(coroutine);
                frameIt = frameWaitingCoroutines.erase(frameIt);
            } else {
                ++frameIt;
            }
        }
    }

    void ScriptEngine::updateProfiling(float deltaTime) {
        // TODO: сбор данных профилирования
    }

    void ScriptEngine::checkForHotReload() {
        static auto lastCheck = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCheck);
        
        if (elapsed.count() > 1000) { // Проверка раз в секунду
            reloadScripts();
            lastCheck = now;
        }
    }

    std::string ScriptEngine::resolveScriptPath(const std::string& path) {
        std::string packagePath = lua["package"]["path"];
        std::istringstream iss(packagePath);
        std::string searchPath;
        
        while (std::getline(iss, searchPath, ';')) {
            // Заменяем '?' на имя файла
            size_t pos = searchPath.find('?');
            if (pos != std::string::npos) {
                std::string fullPath = searchPath.substr(0, pos) + path + searchPath.substr(pos + 1);
                if (fs::exists(fullPath)) {
                    return fullPath;
                }
            }
        }
        
        return "";
    }

    std::string ScriptEngine::loadPackageCode(const std::string& packagePath) {
        // Попытка загрузить как файл
        if (fs::exists(packagePath)) {
            std::ifstream file(packagePath);
            if (file.is_open()) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                return buffer.str();
            }
        }
        
        // Попытка найти по package.path
        std::string resolvedPath = resolveScriptPath(packagePath);
        if (!resolvedPath.empty()) {
            std::ifstream file(resolvedPath);
            if (file.is_open()) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                return buffer.str();
            }
        }
        
        return "";
    }

    sol::coroutine ScriptEngine::createWaitCoroutine(float seconds) {
        sol::function func = lua.load("return coroutine.yield()");
        sol::coroutine coroutine = lua.create_coroutine(func);
        
        waitingCoroutines.emplace_back(coroutine, seconds);
        return coroutine;
    }

    sol::coroutine ScriptEngine::createWaitFramesCoroutine(int frames) {
        sol::function func = lua.load("return coroutine.yield()");
        sol::coroutine coroutine = lua.create_coroutine(func);
        
        frameWaitingCoroutines.emplace_back(coroutine, frames);
        return coroutine;
    }

    sol::coroutine ScriptEngine::createWaitEndOfFrameCoroutine() {
        return createWaitFramesCoroutine(1);
    }

    sol::coroutine ScriptEngine::createWaitUntilCoroutine(sol::function condition) {
        sol::function func = lua.load(R"(
            return function(condition)
                while not condition() do
                    coroutine.yield()
                end
            end
        )");
        
        sol::coroutine coroutine = lua.create_coroutine(func);
        conditionWaitingCoroutines.emplace_back(coroutine, condition);
        return coroutine;
    }

    sol::coroutine ScriptEngine::createWaitWhileCoroutine(sol::function condition) {
        sol::function func = lua.load(R"(
            return function(condition)
                while condition() do
                    coroutine.yield()
                end
            end
        )");
        
        sol::coroutine coroutine = lua.create_coroutine(func);
        conditionWaitingCoroutines.emplace_back(coroutine, condition);
        return coroutine;
    }

    sol::table ScriptEngine::deepCopyTable(const sol::table& original) {
        sol::table copy = lua.create_table();
        
        for (auto& pair : original) {
            sol::object key = pair.first;
            sol::object value = pair.second;
            
            if (value.is<sol::table>()) {
                copy[key] = deepCopyTable(value.as<sol::table>());
            } else {
                copy[key] = value;
            }
        }
        
        return copy;
    }

    void ScriptEngine::saveScriptStates() {
        // TODO: сохранение состояния всех скриптов
    }

    // Вспомогательный класс LuaScript
    class LuaScript : public Script {
    public:
        LuaScript(sol::table instance) : luaInstance(instance) {
            // Инициализация из Lua таблицы
            if (luaInstance.valid()) {
                if (luaInstance["Start"].is<sol::function>()) {
                    startFunc = luaInstance["Start"];
                }
                if (luaInstance["Update"].is<sol::function>()) {
                    updateFunc = luaInstance["Update"];
                }
                if (luaInstance["OnDestroy"].is<sol::function>()) {
                    destroyFunc = luaInstance["OnDestroy"];
                }
                // ... другие методы
            }
        }
        
        void OnStart() override {
            if (startFunc) {
                try {
                    startFunc(luaInstance);
                } catch (const sol::error& e) {
                    SPDLOG_ERROR("LuaScript Start error: {}", e.what());
                }
            }
        }
        
        void update(float deltaTime) override {
            if (updateFunc) {
                try {
                    updateFunc(luaInstance, deltaTime);
                } catch (const sol::error& e) {
                    SPDLOG_ERROR("LuaScript Update error: {}", e.what());
                }
            }
        }
        
        void OnDestroy() override {
            if (destroyFunc) {
                try {
                    destroyFunc(luaInstance);
                } catch (const sol::error& e) {
                    SPDLOG_ERROR("LuaScript OnDestroy error: {}", e.what());
                }
            }
        }
        
        void onHotReload() {
            // TODO: перезагрузка Lua скрипта
        }
        
    private:
        sol::table luaInstance;
        sol::function startFunc;
        sol::function updateFunc;
        sol::function destroyFunc;
        // ... другие функции
    };

} // namespace CmakeProject1
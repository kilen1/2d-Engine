#include "Deserializer.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cctype>

namespace CmakeProject1 {

    struct Deserializer::Node {
        enum class Type {
            Null,
            Boolean,
            Integer,
            Float,
            String,
            Object,
            Array
        };

        Type type = Type::Null;
        std::string key;

        union {
            bool boolValue;
            int intValue;
            float floatValue;
            double doubleValue;
        };
        std::string stringValue;

        std::map<std::string, std::shared_ptr<Node>> children;
        std::vector<std::shared_ptr<Node>> arrayElements;

        Node() : type(Type::Null), doubleValue(0.0) {}
        Node(Type t) : type(t), doubleValue(0.0) {}
    };

    Deserializer::Deserializer() {
        m_root = std::make_shared<Node>(Node::Type::Object);
        m_nodeStack.push_back(m_root);
    }

    Deserializer::~Deserializer() {}

    bool Deserializer::loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return parseJSON(buffer.str());
    }

    bool Deserializer::loadFromString(const std::string& data) {
        return parseJSON(data);
    }

    std::shared_ptr<Deserializer::Node> Deserializer::getCurrentNode() const {
        return m_nodeStack.empty() ? m_root : m_nodeStack.back();
    }

    void Deserializer::pushNode(const std::shared_ptr<Node>& node) {
        m_nodeStack.push_back(node);
    }

    void Deserializer::popNode() {
        if (!m_nodeStack.empty()) {
            m_nodeStack.pop_back();
        }
    }

    bool Deserializer::parseJSON(const std::string& json) {
        try {
            size_t pos = 0;
            m_root = std::make_shared<Node>(Node::Type::Object);
            m_nodeStack.clear();
            m_nodeStack.push_back(m_root);
            m_currentArrayIndex = 0;
            m_currentArrayNode = nullptr;

            return parseValue(m_root, json, pos);
        }
        catch (...) {
            return false;
        }
    }

    bool Deserializer::parseValue(std::shared_ptr<Node> node, const std::string& json, size_t& pos) {
        skipWhitespace(json, pos);

        if (pos >= json.length()) return false;

        char c = json[pos];

        if (c == '{') {
            node->type = Node::Type::Object;
            return parseObject(node, json, pos);
        }
        else if (c == '[') {
            node->type = Node::Type::Array;
            return parseArray(node, json, pos);
        }
        else if (c == '"') {
            node->type = Node::Type::String;
            return parseString(node, json, pos);
        }
        else if (c == 't' || c == 'f') {
            node->type = Node::Type::Boolean;
            return parseBoolean(node, json, pos);
        }
        else if (c == 'n') {
            node->type = Node::Type::Null;
            return parseNull(json, pos);
        }
        else if (c == '-' || (c >= '0' && c <= '9')) {
            return parseNumber(node, json, pos);
        }

        return false;
    }

    bool Deserializer::parseObject(std::shared_ptr<Node> node, const std::string& json, size_t& pos) {
        pos++;
        skipWhitespace(json, pos);

        if (pos >= json.length()) return false;

        while (pos < json.length()) {
            skipWhitespace(json, pos);

            if (json[pos] == '}') {
                pos++;
                return true;
            }

            if (json[pos] != '"') return false;

            std::string key;
            if (!parseString(key, json, pos)) return false;

            skipWhitespace(json, pos);

            if (pos >= json.length() || json[pos] != ':') return false;
            pos++;

            skipWhitespace(json, pos);

            auto child = std::make_shared<Node>();
            child->key = key;

            if (!parseValue(child, json, pos)) return false;

            node->children[key] = child;

            skipWhitespace(json, pos);

            if (json[pos] == ',') {
                pos++;
                continue;
            }
        }

        return false;
    }

    bool Deserializer::parseArray(std::shared_ptr<Node> node, const std::string& json, size_t& pos) {
        pos++;
        skipWhitespace(json, pos);

        if (pos >= json.length()) return false;

        while (pos < json.length()) {
            skipWhitespace(json, pos);

            if (json[pos] == ']') {
                pos++;
                return true;
            }

            auto element = std::make_shared<Node>();
            if (!parseValue(element, json, pos)) return false;

            node->arrayElements.push_back(element);

            skipWhitespace(json, pos);

            if (json[pos] == ',') {
                pos++;
                continue;
            }
        }

        return false;
    }

    bool Deserializer::parseString(std::shared_ptr<Node> node, const std::string& json, size_t& pos) {
        std::string value;
        if (!parseString(value, json, pos)) return false;

        node->stringValue = value;
        return true;
    }

    bool Deserializer::parseString(std::string& result, const std::string& json, size_t& pos) {
        if (pos >= json.length() || json[pos] != '"') return false;

        pos++;
        std::ostringstream ss;

        while (pos < json.length()) {
            char c = json[pos];

            if (c == '"') {
                pos++;
                result = ss.str();
                return true;
            }
            else if (c == '\\') {
                pos++;
                if (pos >= json.length()) return false;

                c = json[pos];
                switch (c) {
                case '"': ss << '"'; break;
                case '\\': ss << '\\'; break;
                case '/': ss << '/'; break;
                case 'b': ss << '\b'; break;
                case 'f': ss << '\f'; break;
                case 'n': ss << '\n'; break;
                case 'r': ss << '\r'; break;
                case 't': ss << '\t'; break;
                case 'u':
                    if (pos + 4 < json.length()) {
                        pos += 4;
                        ss << '?';
                    }
                    else {
                        return false;
                    }
                    break;
                default:
                    ss << c;
                    break;
                }
                pos++;
            }
            else {
                ss << c;
                pos++;
            }
        }

        return false;
    }

    bool Deserializer::parseBoolean(std::shared_ptr<Node> node, const std::string& json, size_t& pos) {
        if (json.compare(pos, 4, "true") == 0) {
            node->boolValue = true;
            pos += 4;
            return true;
        }
        else if (json.compare(pos, 5, "false") == 0) {
            node->boolValue = false;
            pos += 5;
            return true;
        }

        return false;
    }

    bool Deserializer::parseNull(const std::string& json, size_t& pos) {
        if (json.compare(pos, 4, "null") == 0) {
            pos += 4;
            return true;
        }
        return false;
    }

    bool Deserializer::parseNumber(std::shared_ptr<Node> node, const std::string& json, size_t& pos) {
        size_t start = pos;

        bool isFloat = false;

        if (json[pos] == '-') {
            pos++;
        }

        while (pos < json.length()) {
            char c = json[pos];
            if (c == '.' || c == 'e' || c == 'E') {
                isFloat = true;
            }
            else if (c == ',' || c == '}' || c == ']' || std::isspace(c)) {
                break;
            }
            pos++;
        }

        std::string numStr = json.substr(start, pos - start);

        try {
            if (isFloat) {
                node->type = Node::Type::Float;
                node->floatValue = std::stof(numStr);
                node->doubleValue = std::stod(numStr);
            }
            else {
                node->type = Node::Type::Integer;
                node->intValue = std::stoi(numStr);
                node->floatValue = static_cast<float>(node->intValue);
                node->doubleValue = static_cast<double>(node->intValue);
            }
            return true;
        }
        catch (...) {
            return false;
        }
    }

    void Deserializer::skipWhitespace(const std::string& json, size_t& pos) {
        while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) {
            pos++;
        }
    }

    bool Deserializer::read(const std::string& key, bool& value)  {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Boolean) return false;

        value = node->boolValue;
        return true;
    }

    bool Deserializer::read(const std::string& key, int& value)  {
        auto node = findNode(key);
        if (!node) return false;

        if (node->type == Node::Type::Integer) {
            value = node->intValue;
            return true;
        }
        else if (node->type == Node::Type::Float) {
            value = static_cast<int>(node->floatValue);
            return true;
        }

        return false;
    }

    bool Deserializer::read(const std::string& key, float& value)  {
        auto node = findNode(key);
        if (!node) return false;

        if (node->type == Node::Type::Float) {
            value = node->floatValue;
            return true;
        }
        else if (node->type == Node::Type::Integer) {
            value = static_cast<float>(node->intValue);
            return true;
        }

        return false;
    }

    bool Deserializer::read(const std::string& key, double& value)  {
        auto node = findNode(key);
        if (!node) return false;

        if (node->type == Node::Type::Float) {
            value = static_cast<double>(node->floatValue);
            return true;
        }
        else if (node->type == Node::Type::Integer) {
            value = static_cast<double>(node->intValue);
            return true;
        }

        return false;
    }

    bool Deserializer::read(const std::string& key, std::string& value)  {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::String) return false;

        value = node->stringValue;
        return true;
    }

    bool Deserializer::read(const std::string& key, Vector2& value)  {
    if (!beginObject(key)) {
        return false; // Ключ не найден или не объект
    }

    float x = 0, y = 0;
    bool success = true;
    success &= read("x", x);
    success &= read("y", y);

    endObject(); // Всегда вызываем, так как beginObject прошёл

    if (success) {
        value.x = x;
        value.y = y;
        return true;
    }
    return false;
}

bool Deserializer::read(const std::string& key, Vector3& value)  {
    if (!beginObject(key)) {
        return false;
    }

    float x = 0, y = 0, z = 0;
    bool success = true;
    success &= read("x", x);
    success &= read("y", y);
    success &= read("z", z);

    endObject();

    if (success) {
        value.x = x;
        value.y = y;
        value.z = z;
        return true;
    }
    return false;
}

bool Deserializer::read(const std::string& key, Vector4& value)  {
    if (!beginObject(key)) {
        return false;
    }

    float x = 0, y = 0, z = 0, w = 0;
    bool success = true;
    success &= read("x", x);
    success &= read("y", y);
    success &= read("z", z);
    success &= read("w", w);

    endObject();

    if (success) {
        value.x = x;
        value.y = y;
        value.z = z;
        value.w = w;
        return true;
    }
    return false;
}

bool Deserializer::read(const std::string& key, Color& value)  {
    if (!beginObject(key)) {
        return false;
    }

    float r = 0, g = 0, b = 0, a = 1.0f;
    bool success = true;
    success &= read("r", r);
    success &= read("g", g);
    success &= read("b", b);
    read("a", a); // 'a' опциональный — не делаем его обязательным

    endObject();

    if (success) {
        value.r = r;
        value.g = g;
        value.b = b;
        value.a = a;
        return true;
    }
    return false;
}

bool Deserializer::read(const std::string& key, Rect& value)  {
    if (!beginObject(key)) {
        return false;
    }

    float x = 0, y = 0, width = 0, height = 0;
    bool success = true;
    success &= read("x", x);
    success &= read("y", y);
    success &= read("width", width);
    success &= read("height", height);

    endObject();

    if (success) {
        value.x = x;
        value.y = y;
        value.width = width;
        value.height = height;
        return true;
    }
    return false;
}

    bool Deserializer::read(const std::string& key, Matrix4& value)  {
        std::vector<float> values;
        if (!read(key, values)) return false;

        if (values.size() == 16) {
            for (int i = 0; i < 16; i++) {
                value.m[i] = values[i];
            }
            return true;
        }

        return false;
    }

    bool Deserializer::read(const std::string& key, std::vector<int>& values)  {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Array) return false;

        values.clear();
        for (const auto& element : node->arrayElements) {
            if (element->type == Node::Type::Integer) {
                values.push_back(element->intValue);
            }
            else if (element->type == Node::Type::Float) {
                values.push_back(static_cast<int>(element->floatValue));
            }
            else {
                return false;
            }
        }
        return true;
    }

    bool Deserializer::read(const std::string& key, std::vector<float>& values)  {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Array) return false;

        values.clear();
        for (const auto& element : node->arrayElements) {
            if (element->type == Node::Type::Float) {
                values.push_back(element->floatValue);
            }
            else if (element->type == Node::Type::Integer) {
                values.push_back(static_cast<float>(element->intValue));
            }
            else {
                return false;
            }
        }
        return true;
    }

    bool Deserializer::read(const std::string& key, std::vector<std::string>& values)  {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Array) return false;

        values.clear();
        for (const auto& element : node->arrayElements) {
            if (element->type == Node::Type::String) {
                values.push_back(element->stringValue);
            }
            else {
                return false;
            }
        }
        return true;
    }

    bool Deserializer::read(const std::string& key, std::vector<Vector2>& values)  {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Array) return false;

        values.clear();
        for (const auto& element : node->arrayElements) {
            if (element->type == Node::Type::Object) {
                Vector2 vec;
                if (!readElement(element, vec)) {
                    return false;
                }
                values.push_back(vec);
            }
            else {
                return false;
            }
        }
        return true;
    }

    bool Deserializer::read(const std::string& key, std::vector<Vector3>& values)  {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Array) return false;

        values.clear();
        for (const auto& element : node->arrayElements) {
            if (element->type == Node::Type::Object) {
                Vector3 vec;
                if (!readElement(element, vec)) {
                    return false;
                }
                values.push_back(vec);
            }
            else {
                return false;
            }
        }
        return true;
    }

    bool Deserializer::readElement(const std::shared_ptr<Node>& element, Vector2& value)  {
        if (!element || element->type != Node::Type::Object) return false;

        auto originalStack = m_nodeStack;
        m_nodeStack = { element };

        float x = 0, y = 0;
        bool hasX = read("x", x);
        bool hasY = read("y", y);

        m_nodeStack = originalStack;

        if (hasX && hasY) {
            value.x = x;
            value.y = y;
            return true;
        }
        return false;
    }

    bool Deserializer::readElement(const std::shared_ptr<Node>& element, Vector3& value)  {
        if (!element || element->type != Node::Type::Object) return false;

        auto originalStack = m_nodeStack;
        m_nodeStack = { element };

        float x = 0, y = 0, z = 0;
        bool hasX = read("x", x);
        bool hasY = read("y", y);
        bool hasZ = read("z", z);

        m_nodeStack = originalStack;

        if (hasX && hasY && hasZ) {
            value.x = x;
            value.y = y;
            value.z = z;
            return true;
        }
        return false;
    }

    bool Deserializer::beginObject(const std::string& key) {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Object) {
            return false;
        }

        pushNode(node);
        return true;
    }

    void Deserializer::endObject() {
        popNode();
    }

    bool Deserializer::beginArray(const std::string& key) {
        auto node = findNode(key);
        if (!node || node->type != Node::Type::Array) {
            return false;
        }

        m_currentArrayNode = node;
        m_currentArrayIndex = 0;
        return true;
    }

    void Deserializer::endArray() {
        m_currentArrayNode = nullptr;
        m_currentArrayIndex = 0;
    }

    bool Deserializer::nextArrayElement() {
        if (!m_currentArrayNode || m_currentArrayIndex >= m_currentArrayNode->arrayElements.size()) {
            return false;
        }

        auto element = m_currentArrayNode->arrayElements[m_currentArrayIndex++];

        if (!m_nodeStack.empty() && m_nodeStack.back()->type == Node::Type::Object) {
            popNode();
        }

        if (element->type == Node::Type::Object) {
            pushNode(element);
            return true;
        }

        return false;
    }

    std::string Deserializer::getObjectType()  {
        std::string typeName;
        if (read("__type", typeName)) {
            return typeName;
        }
        return "";
    }

    bool Deserializer::readBinary(const std::string& key, void* data, size_t size)  {
        return false;
    }

    bool Deserializer::hasKey(const std::string& key)  {
        return findNode(key) != nullptr;
    }

    std::vector<std::string> Deserializer::getKeys()  {
        std::vector<std::string> keys;
        auto currentNode = getCurrentNode();

        if (currentNode && currentNode->type == Node::Type::Object) {
            for (const auto& [key, _] : currentNode->children) {
                keys.push_back(key);
            }
        }

        return keys;
    }

    std::shared_ptr<Deserializer::Node> Deserializer::findNode(const std::string& key) const {
        auto currentNode = getCurrentNode();

        if (!currentNode) {
            return nullptr;
        }

        if (key.empty()) {
            return currentNode;
        }

        std::string currentKey = key;
        std::string remainingKeys;

        size_t dotPos = key.find('.');
        if (dotPos != std::string::npos) {
            currentKey = key.substr(0, dotPos);
            remainingKeys = key.substr(dotPos + 1);
        }

        auto it = currentNode->children.find(currentKey);
        if (it == currentNode->children.end()) {
            return nullptr;
        }

        if (remainingKeys.empty()) {
            return it->second;
        }

        Deserializer temp;
        temp.m_root = it->second;
        temp.m_nodeStack = { it->second };
        return temp.findNode(remainingKeys);
    }

}
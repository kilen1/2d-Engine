#include "Serializer.h"
#include <fstream>
#include <sstream>
#include <iomanip>

namespace CmakeProject1 {

    struct Serializer::Node {
        enum class Type {
            Null,
            Boolean,
            Integer,
            Float,
            String,
            Vector2,
            Vector3,
            Vector4,
            Color,
            Rect,
            Matrix4,
            Object,
            Array
        };

        Type type = Type::Null;
        std::string key;

        // Value storage
        union {
            bool boolValue;
            int intValue;
            float floatValue;
        };
        std::string stringValue;
        Vector2 vector2Value;
        Vector3 vector3Value;
        Vector4 vector4Value;
        Color colorValue;
        Rect rectValue;
        Matrix4 matrix4Value;

        // Children for objects and arrays
        std::map<std::string, std::shared_ptr<Node>> children;
        std::vector<std::shared_ptr<Node>> arrayElements;

        Node() : type(Type::Null) {}
        Node(Type t) : type(t) {}
    };

    Serializer::Serializer() {
        m_root = std::make_shared<Node>(Node::Type::Object);
        m_nodeStack.push_back(m_root);
    }

    Serializer::~Serializer() {}

    std::shared_ptr<Serializer::Node> Serializer::getCurrentNode() const {
        return m_nodeStack.empty() ? m_root : m_nodeStack.back();
    }

    void Serializer::pushNode(const std::shared_ptr<Node>& node) {
        m_nodeStack.push_back(node);
    }

    void Serializer::popNode() {
        if (!m_nodeStack.empty()) {
            m_nodeStack.pop_back();
        }
    }

    // Write methods implementation
    void Serializer::write(const std::string& key, bool value) {
        auto node = std::make_shared<Node>(Node::Type::Boolean);
        node->key = key;
        node->boolValue = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, int value) {
        auto node = std::make_shared<Node>(Node::Type::Integer);
        node->key = key;
        node->intValue = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, float value) {
        auto node = std::make_shared<Node>(Node::Type::Float);
        node->key = key;
        node->floatValue = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, double value) {
        write(key, static_cast<float>(value));
    }

    void Serializer::write(const std::string& key, const std::string& value) {
        auto node = std::make_shared<Node>(Node::Type::String);
        node->key = key;
        node->stringValue = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, const char* value) {
        write(key, std::string(value));
    }

    void Serializer::write(const std::string& key, const Vector2& value) {
        auto node = std::make_shared<Node>(Node::Type::Vector2);
        node->key = key;
        node->vector2Value = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, const Vector3& value) {
        auto node = std::make_shared<Node>(Node::Type::Vector3);
        node->key = key;
        node->vector3Value = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, const Vector4& value) {
        auto node = std::make_shared<Node>(Node::Type::Vector4);
        node->key = key;
        node->vector4Value = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, const Color& value) {
        auto node = std::make_shared<Node>(Node::Type::Color);
        node->key = key;
        node->colorValue = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, const Rect& value) {
        auto node = std::make_shared<Node>(Node::Type::Rect);
        node->key = key;
        node->rectValue = value;
        getCurrentNode()->children[key] = node;
    }

    void Serializer::write(const std::string& key, const Matrix4& value) {
        auto node = std::make_shared<Node>(Node::Type::Matrix4);
        node->key = key;
        node->matrix4Value = value;
        getCurrentNode()->children[key] = node;
    }

    // Array write methods
    void Serializer::write(const std::string& key, const std::vector<int>& values) {
        auto arrayNode = std::make_shared<Node>(Node::Type::Array);
        arrayNode->key = key;

        for (const auto& value : values) {
            auto element = std::make_shared<Node>(Node::Type::Integer);
            element->intValue = value;
            arrayNode->arrayElements.push_back(element);
        }

        getCurrentNode()->children[key] = arrayNode;
    }

    void Serializer::write(const std::string& key, const std::vector<float>& values) {
        auto arrayNode = std::make_shared<Node>(Node::Type::Array);
        arrayNode->key = key;

        for (const auto& value : values) {
            auto element = std::make_shared<Node>(Node::Type::Float);
            element->floatValue = value;
            arrayNode->arrayElements.push_back(element);
        }

        getCurrentNode()->children[key] = arrayNode;
    }

    void Serializer::write(const std::string& key, const std::vector<std::string>& values) {
        auto arrayNode = std::make_shared<Node>(Node::Type::Array);
        arrayNode->key = key;

        for (const auto& value : values) {
            auto element = std::make_shared<Node>(Node::Type::String);
            element->stringValue = value;
            arrayNode->arrayElements.push_back(element);
        }

        getCurrentNode()->children[key] = arrayNode;
    }

    void Serializer::write(const std::string& key, const std::vector<Vector2>& values) {
        auto arrayNode = std::make_shared<Node>(Node::Type::Array);
        arrayNode->key = key;

        for (const auto& value : values) {
            auto element = std::make_shared<Node>(Node::Type::Vector2);
            element->vector2Value = value;
            arrayNode->arrayElements.push_back(element);
        }

        getCurrentNode()->children[key] = arrayNode;
    }

    void Serializer::write(const std::string& key, const std::vector<Vector3>& values) {
        auto arrayNode = std::make_shared<Node>(Node::Type::Array);
        arrayNode->key = key;

        for (const auto& value : values) {
            auto element = std::make_shared<Node>(Node::Type::Vector3);
            element->vector3Value = value;
            arrayNode->arrayElements.push_back(element);
        }

        getCurrentNode()->children[key] = arrayNode;
    }

    void Serializer::writeObject(const std::string& key, const std::string& typeName) {
        auto objectNode = std::make_shared<Node>(Node::Type::Object);
        objectNode->key = key;

        // Store type name as first property
        auto typeNode = std::make_shared<Node>(Node::Type::String);
        typeNode->key = "__type";
        typeNode->stringValue = typeName;
        objectNode->children["__type"] = typeNode;

        getCurrentNode()->children[key] = objectNode;
        pushNode(objectNode);
    }

    void Serializer::endObject() {
        if (!m_nodeStack.empty() && m_nodeStack.back()->type == Node::Type::Object) {
            popNode();
        }
    }

    void Serializer::writeArray(const std::string& key) {
        auto arrayNode = std::make_shared<Node>(Node::Type::Array);
        arrayNode->key = key;
        getCurrentNode()->children[key] = arrayNode;
        pushNode(arrayNode);
    }

    void Serializer::endArray() {
        if (!m_nodeStack.empty() && m_nodeStack.back()->type == Node::Type::Array) {
            popNode();
        }
    }

    void Serializer::writeBinary(const std::string& key, const void* data, size_t size) {
        // For now, store as base64 string
        // In a real implementation, you'd convert to base64
        auto node = std::make_shared<Node>(Node::Type::String);
        node->key = key;
        node->stringValue = "[BinaryData:" + std::to_string(size) + " bytes]";
        getCurrentNode()->children[key] = node;
    }

    bool Serializer::saveToFile(const std::string& filename) const {
        std::string json = toJSON();
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        file << json;
        file.close();
        return true;
    }

    std::string Serializer::toString() const {
        return toJSON();
    }

    std::string Serializer::toJSON() const {
        std::ostringstream ss;
        ss << std::boolalpha;

        std::function<void(const std::shared_ptr<Node>&, int)> writeNode;
        writeNode = [&](const std::shared_ptr<Node>& node, int indent) {
            std::string indentStr(indent * 2, ' ');

            switch (node->type) {
            case Node::Type::Boolean:
                ss << (node->boolValue ? "true" : "false");
                break;
            case Node::Type::Integer:
                ss << node->intValue;
                break;
            case Node::Type::Float:
                ss << std::fixed << std::setprecision(6) << node->floatValue;
                break;
            case Node::Type::String:
                ss << "\"" << node->stringValue << "\"";
                break;
            case Node::Type::Vector2:
                ss << "{\"x\":" << node->vector2Value.x
                    << ",\"y\":" << node->vector2Value.y << "}";
                break;
            case Node::Type::Vector3:
                ss << "{\"x\":" << node->vector3Value.x
                    << ",\"y\":" << node->vector3Value.y
                    << ",\"z\":" << node->vector3Value.z << "}";
                break;
            case Node::Type::Vector4:
                ss << "{\"x\":" << node->vector4Value.x
                    << ",\"y\":" << node->vector4Value.y
                    << ",\"z\":" << node->vector4Value.z
                    << ",\"w\":" << node->vector4Value.w << "}";
                break;
            case Node::Type::Color:
                ss << "{\"r\":" << node->colorValue.r
                    << ",\"g\":" << node->colorValue.g
                    << ",\"b\":" << node->colorValue.b
                    << ",\"a\":" << node->colorValue.a << "}";
                break;
            case Node::Type::Rect:
                ss << "{\"x\":" << node->rectValue.x
                    << ",\"y\":" << node->rectValue.y
                    << ",\"width\":" << node->rectValue.width
                    << ",\"height\":" << node->rectValue.height << "}";
                break;
            case Node::Type::Matrix4:
                ss << "[";
                for (int i = 0; i < 16; i++) {
                    if (i > 0) ss << ",";
                    ss << node->matrix4Value.m[i];
                }
                ss << "]";
                break;
            case Node::Type::Object:
                ss << "{\n";
                {
                    bool first = true;
                    for (const auto& [key, child] : node->children) {
                        if (!first) ss << ",\n";
                        first = false;

                        ss << indentStr << "  \"" << key << "\": ";
                        writeNode(child, indent + 1);
                    }
                }
                ss << "\n" << indentStr << "}";
                break;
            case Node::Type::Array:
                ss << "[\n";
                for (size_t i = 0; i < node->arrayElements.size(); i++) {
                    if (i > 0) ss << ",\n";
                    ss << indentStr << "  ";
                    writeNode(node->arrayElements[i], indent + 1);
                }
                ss << "\n" << indentStr << "]";
                break;
            case Node::Type::Null:
                ss << "null";
                break;
            }
            };

        writeNode(m_root, 0);
        return ss.str();
    }

} // namespace CmakeProject1

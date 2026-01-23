#pragma once

#include "Types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace CmakeProject1 {

    class Deserializer {
    public:
        Deserializer();
        ~Deserializer();
        
        // Load from file/string
        bool loadFromFile(const std::string& filename);
        bool loadFromString(const std::string& data);

        // Read primitive types
        bool read(const std::string& key, bool& value);
        bool read(const std::string& key, int& value);
        bool read(const std::string& key, float& value);
        bool read(const std::string& key, double& value);
        bool read(const std::string& key, std::string& value);

        // Read custom types
        bool read(const std::string& key, Vector2& value);
        bool read(const std::string& key, Vector3& value);
        bool read(const std::string& key, Vector4& value);
        bool read(const std::string& key, Color& value);
        bool read(const std::string& key, Rect& value);
        bool read(const std::string& key, Matrix4& value);

        // Read array types
        bool read(const std::string& key, std::vector<int>& values);
        bool read(const std::string& key, std::vector<float>& values);
        bool read(const std::string& key, std::vector<std::string>& values);
        bool read(const std::string& key, std::vector<Vector2>& values);
        bool read(const std::string& key, std::vector<Vector3>& values);

        // Object navigation
        bool beginObject(const std::string& key);
        void endObject();

        bool beginArray(const std::string& key);
        void endArray();

        bool nextArrayElement();
        std::string getObjectType() ;

        // Binary data
        bool readBinary(const std::string& key, void* data, size_t size) ;

        // Check if key exists
        bool hasKey(const std::string& key) ;

        // Get all keys
        std::vector<std::string> getKeys() ;

    private:
        struct Node;
        std::shared_ptr<Node> m_root;
        std::vector<std::shared_ptr<Node>> m_nodeStack;

        std::shared_ptr<Node> getCurrentNode() const;
        void pushNode(const std::shared_ptr<Node>& node);
        void popNode();

        // JSON parsing helper functions
        bool parseJSON(const std::string& json);
        bool parseValue(std::shared_ptr<Node> node, const std::string& json, size_t& pos);
        bool parseObject(std::shared_ptr<Node> node, const std::string& json, size_t& pos);
        bool parseArray(std::shared_ptr<Node> node, const std::string& json, size_t& pos);
        bool parseString(std::shared_ptr<Node> node, const std::string& json, size_t& pos);
        bool parseString(std::string& result, const std::string& json, size_t& pos);
        bool parseBoolean(std::shared_ptr<Node> node, const std::string& json, size_t& pos);
        bool parseNull(const std::string& json, size_t& pos);
        bool parseNumber(std::shared_ptr<Node> node, const std::string& json, size_t& pos);
        void skipWhitespace(const std::string& json, size_t& pos);

        // Node search helper
        std::shared_ptr<Node> findNode(const std::string& key) const;

        // Helper methods for reading elements from arrays
        bool readElement(const std::shared_ptr<Node>& element, Vector2& value) ;
        bool readElement(const std::shared_ptr<Node>& element, Vector3& value) ;

        // Current array index for iteration
        size_t m_currentArrayIndex = 0;
        std::shared_ptr<Node> m_currentArrayNode = nullptr;
    };

}
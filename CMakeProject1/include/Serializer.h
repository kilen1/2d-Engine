#pragma once

#include "Types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <Transform.h>



namespace CmakeProject1 {

    class Serializer {
    public:
        Serializer();
        ~Serializer();

        void write(const std::string& key, const Transform& value);
        // Write methods
        void write(const std::string& key, bool value);
        void write(const std::string& key, int value);
        void write(const std::string& key, float value);
        void write(const std::string& key, double value);
        void write(const std::string& key, const std::string& value);
        void write(const std::string& key, const char* value);
        void write(const std::string& key, const Vector2& value);
        void write(const std::string& key, const Vector3& value);
        void write(const std::string& key, const Vector4& value);
        void write(const std::string& key, const Color& value);
        void write(const std::string& key, const Rect& value);
        void write(const std::string& key, const Matrix4& value);

        // Write array methods
        void write(const std::string& key, const std::vector<int>& values);
        void write(const std::string& key, const std::vector<float>& values);
        void write(const std::string& key, const std::vector<std::string>& values);
        void write(const std::string& key, const std::vector<Vector2>& values);
        void write(const std::string& key, const std::vector<Vector3>& values);

        // Write object
        void writeObject(const std::string& key, const std::string& typeName);
        void endObject();

        // Write array of objects
        void writeArray(const std::string& key);
        void endArray();

        // Write binary data
        void writeBinary(const std::string& key, const void* data, size_t size);

        // Save to file/string
        bool saveToFile(const std::string& filename) const;
        std::string toString() const;

        // JSON support
        std::string toJSON() const;

    private:
        struct Node;
        std::shared_ptr<Node> m_root;
        std::vector<std::shared_ptr<Node>> m_nodeStack;

        std::shared_ptr<Node> getCurrentNode() const;
        void pushNode(const std::shared_ptr<Node>& node);
        void popNode();

        // Binary serialization support
        bool m_binaryMode = false;
        std::vector<char> m_binaryData;

        friend class Deserializer;
    };

} // namespace CmakeProject1
#include <Transform.h>
#include <GameObject.h>
#include <Serializer.h>
#include <Deserializer.h>

#include <memory>
namespace CmakeProject1 {

    Transform::Transform() {
        
        setDirty();
    }

    void Transform::setPosition(const Vector2& position) {
        m_position = position;
        setDirty();
    }

    void Transform::setPosition(float x, float y) {
        m_position = Vector2(x, y);
        setDirty();
    }

    void Transform::translate(const Vector2& translation) {
        m_position = m_position + translation;
        setDirty();
    }

    void Transform::setRotation(float degrees) {
        m_rotation = degrees;
        setDirty();
    }

    void Transform::rotate(float degrees) {
        m_rotation += degrees;
        setDirty();
    }

    void Transform::setScale(const Vector2& scale) {
        m_scale = scale;
        setDirty();
    }

    void Transform::setScale(float x, float y) {
        m_scale = Vector2(x, y);
        setDirty();
    }

    void Transform::setUniformScale(float scale) {
        m_scale = Vector2(scale, scale);
        setDirty();
    }

    Vector2 Transform::getWorldPosition() const {
       
        if (auto parent = getParent()) {
            Matrix4 worldMatrix = getWorldMatrix();
            return Vector2(worldMatrix.m[12], worldMatrix.m[13]);
        }
        return m_position;
    }

    void Transform::setWorldPosition(const Vector2& position) {
        
        if (auto parent = getParent()) {
            Matrix4 invParentMatrix = parent->getWorldMatrix();
            
            Vector2 localPos = position - parent->getWorldPosition();
            setPosition(localPos);
        } else {
            setPosition(position);
        }
    }

    float Transform::getWorldRotation() const {
        if (auto parent = getParent()) {
            return parent->getWorldRotation() + m_rotation;
        }
        return m_rotation;
    }

    void Transform::setWorldRotation(float degrees) {
        if (auto parent = getParent()) {
            setRotation(degrees - parent->getWorldRotation());
        } else {
            setRotation(degrees);
        }
    }

    Vector2 Transform::getWorldScale() const {
        if (auto parent = getParent()) {
            Vector2 parentScale = parent->getWorldScale();
            return Vector2(m_scale.x * parentScale.x, m_scale.y * parentScale.y);
        }
        return m_scale;
    }

    Matrix4 Transform::getLocalMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_localMatrix;
    }

    Matrix4 Transform::getWorldMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_worldMatrix;
    }

    Vector2 Transform::getRight() const {
        float rad = m_rotation * M_PI / 180.0f;
        return Vector2(cos(rad), sin(rad));
    }

    Vector2 Transform::getUp() const {
        float rad = (m_rotation + 90) * M_PI / 180.0f;
        return Vector2(cos(rad), sin(rad));
    }

    Vector2 Transform::getForward() const {
        
        float rad = m_rotation * M_PI / 180.0f;
        return Vector2(-sin(rad), cos(rad));
    }

    Vector2 Transform::transformPoint(const Vector2& point) const {
        Matrix4 worldMatrix = getWorldMatrix();
       
        float x = worldMatrix.m[0] * point.x + worldMatrix.m[4] * point.y + worldMatrix.m[12];
        float y = worldMatrix.m[1] * point.x + worldMatrix.m[5] * point.y + worldMatrix.m[13];
        return Vector2(x, y);
    }

    Vector2 Transform::inverseTransformPoint(const Vector2& point) const {
        Matrix4 invMatrix = getWorldMatrix();
       
        return Vector2(point.x - m_position.x, point.y - m_position.y);
    }

    Vector2 Transform::transformDirection(const Vector2& direction) const {
        Matrix4 worldMatrix = getWorldMatrix();
       
        float x = worldMatrix.m[0] * direction.x + worldMatrix.m[4] * direction.y;
        float y = worldMatrix.m[1] * direction.x + worldMatrix.m[5] * direction.y;
        return Vector2(x, y);
    }

    Vector2 Transform::inverseTransformDirection(const Vector2& direction) const {
        Vector2 scale = getWorldScale();
     
        float invX = (scale.x != 0.0f) ? direction.x / scale.x : 0.0f;
        float invY = (scale.y != 0.0f) ? direction.y / scale.y : 0.0f;
        return Vector2(invX, invY);
    }

    void Transform::lookAt(const Vector2& target) {
        Vector2 direction = target - getWorldPosition();
        float angle = atan2(direction.y, direction.x) * 180.0f / M_PI;
        setRotation(angle);
    }

    void Transform::lookAt(const Transform& target) {
        lookAt(target.getWorldPosition());
    }

    std::shared_ptr<Transform> Transform::getChild(int index) const {
        if (index >= 0 && index < static_cast<int>(m_children.size())) {
            return m_children[index].lock();
        }
        return nullptr;
    }

    int Transform::getChildCount() const {
        return static_cast<int>(m_children.size());
    }

    std::shared_ptr<Transform> Transform::getParent() const {
        return m_parent.lock();
    }

    void Transform::setParent(std::shared_ptr<Transform> parent) {
        setParent(parent, true);
    }

    void Transform::setParent(std::shared_ptr<Transform> parent, bool worldPositionStays) {
        if (parent == shared_from_this()) return;
        
     
        Vector2 oldWorldPos = getWorldPosition();
        float oldWorldRot = getWorldRotation();
        Vector2 oldWorldScale = getWorldScale();
        
      
        if (auto currentParent = getParent()) {
            auto it = std::find_if(currentParent->m_children.begin(), currentParent->m_children.end(),
                [this](const std::weak_ptr<Transform>& wp) {
                    return wp.lock() == shared_from_this();
                });
            if (it != currentParent->m_children.end()) {
                currentParent->m_children.erase(it);
            }
        }
        
      
        m_parent = parent;
        
       
        if (parent) {
            parent->m_children.push_back(shared_from_this());
        }
        
     
        if (worldPositionStays) {
            setWorldPosition(oldWorldPos);
            setWorldRotation(oldWorldRot - (parent ? parent->getWorldRotation() : 0));
          
            Vector2 parentScale = parent ? parent->getWorldScale() : Vector2(1, 1);
            setScale(oldWorldScale.x / parentScale.x, oldWorldScale.y / parentScale.y);
        }
        
        setDirty();
    }

    void Transform::serialize(Serializer& serializer) const {
        Component::serialize(serializer);
        serializer.write("position", m_position);
        serializer.write("rotation", m_rotation);
        serializer.write("scale", m_scale);
    }

    void Transform::deserialize(Deserializer& deserializer) {
        Component::deserialize(deserializer);
        deserializer.read("position", m_position);
        deserializer.read("rotation", m_rotation);
        deserializer.read("scale", m_scale);
        setDirty();
    }

    void Transform::onInspectorGUI() {
       
    }

    void Transform::setDirty() {
        m_dirty = true;
       
        for (const auto& childWeak : m_children) {
            if (auto child = childWeak.lock()) {
                child->setDirty();
            }
        }
    }

    void Transform::updateMatrices() const {
        
        Matrix4 scaleMat;
        scaleMat.m[0] = m_scale.x;
        scaleMat.m[5] = m_scale.y;
        
      
        float rad = m_rotation * M_PI / 180.0f;
        Matrix4 rotMat;
        rotMat.m[0] = cos(rad);
        rotMat.m[1] = sin(rad);
        rotMat.m[4] = -sin(rad);
        rotMat.m[5] = cos(rad);
        
      
        Matrix4 transMat;
        transMat.m[12] = m_position.x;
        transMat.m[13] = m_position.y;
        
        
        m_localMatrix = transMat;
       
        for (int i = 0; i < 4; ++i) {
            float temp[4] = { 0 };
            for (int j = 0; j < 4; ++j) {
                temp[j] = 0;
                for (int k = 0; k < 4; ++k) {
                    if (j == 0) temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                    else if (j == 1) temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                    else if (j == 2) temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                    else temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                }
            }
            for (int j = 0; j < 4; ++j) {
                m_localMatrix.m[i * 4 + j] = temp[j];
            }
        }
       
        for (int i = 0; i < 4; ++i) {
            float temp[4] = { 0 };
            for (int j = 0; j < 4; ++j) {
                temp[j] = 0;
                for (int k = 0; k < 4; ++k) {
                    if (j == 0) temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                    else if (j == 1) temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                    else if (j == 2) temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                    else temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                }
            }
            for (int j = 0; j < 4; ++j) {
                m_localMatrix.m[i * 4 + j] = temp[j];
            }
        }
        
        
        if (auto parent = getParent()) {
            Matrix4 parentWorld = parent->getWorldMatrix();
            
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    m_worldMatrix.m[i * 4 + j] = 0;
                    for (int k = 0; k < 4; ++k) {
                        m_worldMatrix.m[i * 4 + j] += parentWorld.m[i * 4 + k] * m_localMatrix.m[k * 4 + j];
                    }
                }
            }
        } else {
            m_worldMatrix = m_localMatrix;
        }
        
        m_dirty = false;
    }

}
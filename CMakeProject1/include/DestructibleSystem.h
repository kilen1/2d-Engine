#pragma once

namespace CmakeProject1 {

class DestructibleSystem {
public:
    static DestructibleSystem& getInstance();
    
    struct DestructibleObject {
        int health;
        bool isDestroyed;
        // Additional destruction data
    };
    
    void registerDestructible(DestructibleObject* obj);
    void damageObject(DestructibleObject* obj, int damage);
    void updateDestruction();
    void clearDestructibles();

private:
    DestructibleSystem() = default;
    ~DestructibleSystem() = default;
    DestructibleSystem(const DestructibleSystem&) = delete;
    DestructibleSystem& operator=(const DestructibleSystem&) = delete;
    
    std::vector<DestructibleObject*> destructibles;
};

} // namespace CmakeProject1
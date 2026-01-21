#pragma once

#include "Math/Vector2.h"
#include "Math/Rect.h"
#include "Math/Circle.h"
#include "Math/Polygon.h"
#include "Core/Component.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <functional>

namespace CmakeProject1 {

    // Физические слои
    struct PhysicsLayer {
        std::string name;
        int bitmask;
        bool collideWithDefault = true;
    };

    // Типы коллайдеров
    enum class ColliderType {
        Box,
        Circle,
        Polygon,
        Edge,
        Capsule,
        Composite
    };

    // Материал для физики
    struct PhysicsMaterial {
        float density = 1.0f;
        float friction = 0.4f;
        float restitution = 0.2f; // упругость
        float restitutionThreshold = 1.0f;
        
        // Поверхностные свойства
        bool isIce = false;
        bool isSticky = false;
        bool isBouncy = false;
        
        // Звуковые эффекты
        std::string impactSound;
        std::string slidingSound;
        std::string rollingSound;
    };

    // Результат рейкаста
    struct RaycastHit {
        GameObject* gameObject = nullptr;
        Vector2 point;
        Vector2 normal;
        float distance = 0.0f;
        float fraction = 0.0f;
        bool isTrigger = false;
        void* collider = nullptr;
        
        // Дополнительная информация
        Vector2 centroid;
        Vector2 velocity;
        float angularVelocity = 0.0f;
        PhysicsMaterial* material = nullptr;
    };

    // Контакт коллизии
    struct ContactPoint {
        Vector2 point;
        Vector2 normal;
        float separation = 0.0f;
        float impulse = 0.0f;
        float tangentImpulse = 0.0f;
        float friction = 0.0f;
        bool isNew = true;
        bool persistent = false;
    };

    // Сустав (джоинт)
    enum class JointType {
        Revolute,     // вращательный
        Prismatic,    // призматический
        Distance,     // дистанционный
        Pulley,       // блоковый
        Gear,         // зубчатый
        Wheel,        // колесный
        Weld,         // сварной
        Friction,     // фрикционный
        Rope,         // веревочный
        Motor,        // мотор
        Mouse         // мышиный (для драга)
    };

    class Rigidbody2D : public Component {
    public:
        Rigidbody2D();
        ~Rigidbody2D();
        
        // Свойства
        enum class BodyType {
            Static,     // Не двигается
            Dynamic,    // Полная физика
            Kinematic   // Двигается кодом
        };
        
        BodyType getBodyType() const { return bodyType; }
        void setBodyType(BodyType type);
        
        // Параметры движения
        Vector2 velocity;
        float angularVelocity = 0.0f;
        float linearDamping = 0.0f;
        float angularDamping = 0.05f;
        
        // Масса и инерция
        float mass = 1.0f;
        float inverseMass = 1.0f;
        float inertia = 1.0f;
        float inverseInertia = 1.0f;
        
        // Силы и импульсы
        void AddForce(const Vector2& force, ForceMode mode = ForceMode::Force);
        void AddForceAtPosition(const Vector2& force, const Vector2& position, ForceMode mode = ForceMode::Force);
        void AddTorque(float torque, ForceMode mode = ForceMode::Force);
        void AddRelativeForce(const Vector2& force, ForceMode mode = ForceMode::Force);
        
        void AddImpulse(const Vector2& impulse);
        void AddImpulseAtPosition(const Vector2& impulse, const Vector2& position);
        void AddAngularImpulse(float impulse);
        
        // Позиция и вращение
        void SetPosition(const Vector2& position, bool wake = true);
        void SetRotation(float rotation, bool wake = true);
        void MovePosition(const Vector2& position);
        void MoveRotation(float rotation);
        
        // Состояние
        bool IsAwake() const { return isAwake; }
        void WakeUp();
        void Sleep();
        
        bool IsBullet() const { return isBullet; }
        void SetBullet(bool bullet);
        
        bool IsFixedRotation() const { return fixedRotation; }
        void SetFixedRotation(bool fixed);
        
        // Ограничители скорости
        void SetLinearVelocity(const Vector2& velocity);
        void SetAngularVelocity(float velocity);
        
        Vector2 GetLinearVelocity() const { return velocity; }
        float GetAngularVelocity() const { return angularVelocity; }
        
        // Констрейнты
        void FreezePositionX(bool freeze);
        void FreezePositionY(bool freeze);
        void FreezeRotation(bool freeze);
        
        // Коллизии
        bool IsColliding() const { return !contacts.empty(); }
        const std::vector<ContactPoint>& GetContacts() const { return contacts; }
        
        // Суставы
        void CreateJoint(JointType type, Rigidbody2D* otherBody, const JointConfig& config);
        void DestroyJoint(Joint* joint);
        
        // События
        std::function<void(Rigidbody2D* other, const ContactPoint& contact)> onCollisionEnter;
        std::function<void(Rigidbody2D* other, const ContactPoint& contact)> onCollisionStay;
        std::function<void(Rigidbody2D* other)> onCollisionExit;
        std::function<void(Rigidbody2D* other)> onTriggerEnter;
        std::function<void(Rigidbody2D* other)> onTriggerStay;
        std::function<void(Rigidbody2D* other)> onTriggerExit;
        
        // Физический материал
        void SetPhysicsMaterial(const PhysicsMaterial& material);
        PhysicsMaterial* GetPhysicsMaterial() const { return material; }
        
        // Слои
        void SetLayer(int layer);
        int GetLayer() const { return layer; }
        void SetLayerMask(uint32_t mask);
        uint32_t GetLayerMask() const { return layerMask; }
        
        // Дебаг
        void DrawGizmos();
        
        // Сериализация
        void Serialize(Json::Value& json) const override;
        void Deserialize(const Json::Value& json) override;
        
    private:
        BodyType bodyType = BodyType::Dynamic;
        Vector2 position;
        float rotation = 0.0f;
        
        Vector2 forceAccumulator;
        float torqueAccumulator = 0.0f;
        
        bool isAwake = true;
        bool isBullet = false;
        bool fixedRotation = false;
        bool allowSleep = true;
        
        float sleepTimer = 0.0f;
        float sleepThreshold = 0.1f;
        
        std::vector<ContactPoint> contacts;
        std::vector<Joint*> joints;
        
        PhysicsMaterial* material = nullptr;
        std::unique_ptr<PhysicsMaterial> defaultMaterial;
        
        int layer = 0;
        uint32_t layerMask = 0xFFFFFFFF;
        
        std::vector<Collider*> colliders;
        
        friend class PhysicsEngine;
        friend class CollisionSolver;
    };

    class Collider2D : public Component {
    public:
        Collider2D();
        virtual ~Collider2D();
        
        ColliderType GetType() const { return type; }
        
        // Геометрия
        virtual Rect GetBounds() const = 0;
        virtual bool Contains(const Vector2& point) const = 0;
        virtual bool Intersects(const Collider2D* other) const = 0;
        virtual bool Raycast(const Vector2& origin, const Vector2& direction, 
                            float maxDistance, RaycastHit* hit = nullptr) const = 0;
        
        // Трансформация
        void SetOffset(const Vector2& offset);
        Vector2 GetOffset() const { return offset; }
        
        void SetSize(const Vector2& size);
        Vector2 GetSize() const { return size; }
        
        void SetRadius(float radius);
        float GetRadius() const { return radius; }
        
        // Вершины (для полигонов)
        void SetVertices(const std::vector<Vector2>& vertices);
        const std::vector<Vector2>& GetVertices() const { return vertices; }
        
        // Физические свойства
        bool IsTrigger() const { return isTrigger; }
        void SetTrigger(bool trigger);
        
        bool IsSensor() const { return isSensor; }
        void SetSensor(bool sensor);
        
        // Материал
        void SetPhysicsMaterial(const PhysicsMaterial& material);
        PhysicsMaterial* GetPhysicsMaterial() const { return material; }
        
        // Слои
        void SetLayer(int layer);
        int GetLayer() const { return layer; }
        
        void SetLayerMask(uint32_t mask);
        uint32_t GetLayerMask() const { return layerMask; }
        
        // Контакты
        bool IsColliding() const { return !contacts.empty(); }
        const std::vector<ContactPoint>& GetContacts() const { return contacts; }
        
        // Дебаг
        virtual void DrawGizmos();
        
        // Сериализация
        virtual void Serialize(Json::Value& json) const override;
        virtual void Deserialize(const Json::Value& json) override;
        
    protected:
        ColliderType type;
        Vector2 offset = Vector2::Zero;
        Vector2 size = Vector2::One;
        float radius = 0.5f;
        
        std::vector<Vector2> vertices;
        std::vector<Vector2> worldVertices;
        
        bool isTrigger = false;
        bool isSensor = false;
        bool needsUpdate = true;
        
        PhysicsMaterial* material = nullptr;
        std::unique_ptr<PhysicsMaterial> defaultMaterial;
        
        int layer = 0;
        uint32_t layerMask = 0xFFFFFFFF;
        
        std::vector<ContactPoint> contacts;
        
        Rigidbody2D* attachedBody = nullptr;
        
        void UpdateVertices();
        
        friend class PhysicsEngine;
        friend class Rigidbody2D;
    };

    class BoxCollider2D : public Collider2D {
    public:
        BoxCollider2D();
        
        Rect GetBounds() const override;
        bool Contains(const Vector2& point) const override;
        bool Intersects(const Collider2D* other) const override;
        bool Raycast(const Vector2& origin, const Vector2& direction, 
                    float maxDistance, RaycastHit* hit = nullptr) const override;
        
        void SetSize(const Vector2& newSize);
        
    private:
        Vector2 halfSize;
    };

    class CircleCollider2D : public Collider2D {
    public:
        CircleCollider2D();
        
        Rect GetBounds() const override;
        bool Contains(const Vector2& point) const override;
        bool Intersects(const Collider2D* other) const override;
        bool Raycast(const Vector2& origin, const Vector2& direction, 
                    float maxDistance, RaycastHit* hit = nullptr) const override;
        
        void SetRadius(float newRadius);
        
    private:
        Vector2 center;
    };

    class PolygonCollider2D : public Collider2D {
    public:
        PolygonCollider2D();
        
        Rect GetBounds() const override;
        bool Contains(const Vector2& point) const override;
        bool Intersects(const Collider2D* other) const override;
        bool Raycast(const Vector2& origin, const Vector2& direction, 
                    float maxDistance, RaycastHit* hit = nullptr) const override;
        
        void SetVertices(const std::vector<Vector2>& newVertices);
        
        // Выпуклая оболочка
        void CreateConvexHull(const std::vector<Vector2>& points);
        
        // Правильные полигоны
        void CreateRectangle(float width, float height);
        void CreateTriangle(float side);
        void CreateHexagon(float radius);
        
        // Редактирование
        void AddVertex(const Vector2& vertex);
        void RemoveVertex(int index);
        void ClearVertices();
        
    private:
        std::vector<Vector2> normals;
        bool isConvex = true;
        
        void CalculateNormals();
        void EnsureConvex();
    };

    class EdgeCollider2D : public Collider2D {
    public:
        EdgeCollider2D();
        
        Rect GetBounds() const override;
        bool Contains(const Vector2& point) const override;
        bool Intersects(const Collider2D* other) const override;
        bool Raycast(const Vector2& origin, const Vector2& direction, 
                    float maxDistance, RaycastHit* hit = nullptr) const override;
        
        void SetPoints(const std::vector<Vector2>& points);
        const std::vector<Vector2>& GetPoints() const { return vertices; }
        
        // Односторонняя платформа
        bool IsOneWay() const { return isOneWay; }
        void SetOneWay(bool oneWay);
        
        float GetOneWayAngle() const { return oneWayAngle; }
        void SetOneWayAngle(float angle);
        
    private:
        bool isOneWay = false;
        float oneWayAngle = 0.0f; // в градусах
    };

    class CapsuleCollider2D : public Collider2D {
    public:
        CapsuleCollider2D();
        
        enum class Direction {
            Horizontal,
            Vertical
        };
        
        Rect GetBounds() const override;
        bool Contains(const Vector2& point) const override;
        bool Intersects(const Collider2D* other) const override;
        bool Raycast(const Vector2& origin, const Vector2& direction, 
                    float maxDistance, RaycastHit* hit = nullptr) const override;
        
        void SetSize(const Vector2& newSize);
        void SetDirection(Direction dir);
        Direction GetDirection() const { return direction; }
        
    private:
        Direction direction = Direction::Vertical;
        Vector2 capsuleSize;
        
        std::vector<Vector2> GetCapsulePoints() const;
    };

    class CompositeCollider2D : public Collider2D {
    public:
        CompositeCollider2D();
        ~CompositeCollider2D();
        
        Rect GetBounds() const override;
        bool Contains(const Vector2& point) const override;
        bool Intersects(const Collider2D* other) const override;
        bool Raycast(const Vector2& origin, const Vector2& direction, 
                    float maxDistance, RaycastHit* hit = nullptr) const override;
        
        void AddCollider(Collider2D* collider);
        void RemoveCollider(Collider2D* collider);
        void ClearColliders();
        
        const std::vector<Collider2D*>& GetColliders() const { return childColliders; }
        
    private:
        std::vector<Collider2D*> childColliders;
        bool ownsColliders = true;
    };

    class PhysicsEngine {
    public:
        PhysicsEngine();
        ~PhysicsEngine();
        
        bool initialize();
        void shutdown();
        
        void update(float deltaTime);
        void fixedUpdate(float fixedDeltaTime);
        
        // Мир
        void SetGravity(const Vector2& gravity);
        Vector2 GetGravity() const { return gravity; }
        
        void SetIterations(int velocityIterations, int positionIterations);
        void GetIterations(int& velocityIterations, int& positionIterations) const;
        
        void SetEnableSleep(bool enable);
        bool GetEnableSleep() const { return enableSleep; }
        
        void SetWarmStarting(bool enable);
        bool GetWarmStarting() const { return warmStarting; }
        
        void SetContinuousCollision(bool enable);
        bool GetContinuousCollision() const { return continuousCollision; }
        
        // Запросы
        std::vector<RaycastHit> Raycast(const Vector2& origin, const Vector2& direction, 
                                       float distance, uint32_t layerMask = 0xFFFFFFFF);
        std::vector<RaycastHit> RaycastAll(const Vector2& origin, const Vector2& direction, 
                                          float distance, uint32_t layerMask = 0xFFFFFFFF);
        
        RaycastHit Linecast(const Vector2& start, const Vector2& end, 
                           uint32_t layerMask = 0xFFFFFFFF);
        std::vector<RaycastHit> LinecastAll(const Vector2& start, const Vector2& end, 
                                           uint32_t layerMask = 0xFFFFFFFF);
        
        std::vector<Collider2D*> OverlapPoint(const Vector2& point, 
                                             uint32_t layerMask = 0xFFFFFFFF);
        std::vector<Collider2D*> OverlapCircle(const Vector2& center, float radius, 
                                              uint32_t layerMask = 0xFFFFFFFF);
        std::vector<Collider2D*> OverlapBox(const Vector2& center, const Vector2& size, 
                                           float angle = 0.0f, 
                                           uint32_t layerMask = 0xFFFFFFFF);
        std::vector<Collider2D*> OverlapArea(const Vector2& min, const Vector2& max, 
                                            uint32_t layerMask = 0xFFFFFFFF);
        
        bool CheckCollision(Collider2D* a, Collider2D* b);
        
        // Тела
        Rigidbody2D* CreateRigidbody(GameObject* gameObject);
        void DestroyRigidbody(Rigidbody2D* body);
        
        // Коллайдеры
        void AddCollider(Collider2D* collider);
        void RemoveCollider(Collider2D* collider);
        
        // Суставы
        Joint* CreateJoint(JointType type, Rigidbody2D* bodyA, Rigidbody2D* bodyB, 
                          const JointConfig& config);
        void DestroyJoint(Joint* joint);
        
        // Слои
        int CreateLayer(const std::string& name);
        void SetLayerCollision(int layer1, int layer2, bool collide);
        bool GetLayerCollision(int layer1, int layer2) const;
        int GetLayerByName(const std::string& name) const;
        std::string GetLayerName(int layer) const;
        
        // Материалы
        PhysicsMaterial* CreateMaterial(const std::string& name, float density = 1.0f, 
                                       float friction = 0.4f, float restitution = 0.2f);
        PhysicsMaterial* GetMaterial(const std::string& name) const;
        void DestroyMaterial(const std::string& name);
        
        // Дебаг
        void DrawDebug();
        void EnableDebugDraw(bool enable);
        void SetDebugDrawFlags(uint32_t flags);
        
        // Статистика
        struct Statistics {
            int bodyCount = 0;
            int colliderCount = 0;
            int contactCount = 0;
            int jointCount = 0;
            float simulationTime = 0.0f;
            float collisionTime = 0.0f;
            float solverTime = 0.0f;
            float broadphaseTime = 0.0f;
        };
        
        Statistics GetStatistics() const { return stats; }
        
        // События
        void SubscribeCollisionEvent(const std::string& eventType, 
                                    std::function<void(CollisionEvent*)> callback);
        void UnsubscribeCollisionEvent(const std::string& eventType, size_t id);
        
        // Запись/воспроизведение
        void StartRecording();
        void StopRecording();
        void SaveRecording(const std::string& path);
        void LoadRecording(const std::string& path);
        void PlayRecording();
        void PauseRecording();
        
        // Производительность
        void SetThreadCount(int count);
        int GetThreadCount() const { return threadCount; }
        
        void EnableMultithreading(bool enable);
        bool IsMultithreadingEnabled() const { return multithreading; }
        
        // Пространственное разбиение
        enum class BroadphaseType {
            BruteForce,
            SpatialHash,
            DynamicTree,
            SweepAndPrune,
            QuadTree,
            Grid
        };
        
        void SetBroadphaseType(BroadphaseType type);
        BroadphaseType GetBroadphaseType() const { return broadphaseType; }
        
        // Телепортация
        void TeleportBody(Rigidbody2D* body, const Vector2& position, float rotation);
        
        // Запросы AABB
        void QueryAABB(const Rect& aabb, std::vector<Collider2D*>& results, 
                      uint32_t layerMask = 0xFFFFFFFF);
        void QueryCircle(const Vector2& center, float radius, 
                        std::vector<Collider2D*>& results, 
                        uint32_t layerMask = 0xFFFFFFFF);
        void QueryRay(const Vector2& origin, const Vector2& direction, float distance, 
                     std::vector<RaycastHit>& results, 
                     uint32_t layerMask = 0xFFFFFFFF);
        
        // Регионы
        void AddRegion(const Rect& bounds, int layer, bool staticOnly = false);
        void RemoveRegion(const Rect& bounds);
        void ClearRegions();
        
        // Сон/пробуждение
        void PutAllBodiesToSleep();
        void WakeUpAllBodies();
        
        // Пул объектов
        void PreallocateBodies(int count);
        void PreallocateColliders(int count);
        void ClearPool();
        
        // Сериализация
        void Serialize(Json::Value& json) const;
        void Deserialize(const Json::Value& json);
        
        // Тестирование
        void RunUnitTests();
        void Benchmark(int iterations = 1000);
        void StressTest(int bodyCount = 1000);
        
    private:
        Vector2 gravity = Vector2(0.0f, -9.81f);
        float fixedDeltaTime = 1.0f / 60.0f;
        
        int velocityIterations = 8;
        int positionIterations = 3;
        
        bool enableSleep = true;
        bool warmStarting = true;
        bool continuousCollision = true;
        bool autoClearForces = true;
        
        BroadphaseType broadphaseType = BroadphaseType::DynamicTree;
        
        // Системы
        std::unique_ptr<Broadphase> broadphase;
        std::unique_ptr<CollisionSolver> collisionSolver;
        std::unique_ptr<ContactManager> contactManager;
        std::unique_ptr<IslandManager> islandManager;
        std::unique_ptr<JointManager> jointManager;
        
        // Коллекции
        std::vector<Rigidbody2D*> bodies;
        std::vector<Collider2D*> colliders;
        std::vector<Joint*> joints;
        
        // Слои
        std::vector<PhysicsLayer> layers;
        std::unordered_map<std::string, int> layerMap;
        uint32_t layerCollisionMatrix[32];
        
        // Материалы
        std::unordered_map<std::string, std::unique_ptr<PhysicsMaterial>> materials;
        
        // Дебаг
        bool debugDrawEnabled = false;
        uint32_t debugDrawFlags = 0;
        
        // Статистика
        Statistics stats;
        std::chrono::high_resolution_clock::time_point lastStatUpdate;
        
        // Многопоточность
        int threadCount = 1;
        bool multithreading = false;
        std::vector<std::thread> workerThreads;
        std::vector<std::function<void()>> tasks;
        std::mutex taskMutex;
        std::condition_variable taskCondition;
        std::atomic<bool> workersRunning{false};
        
        // Запись
        bool isRecording = false;
        std::vector<PhysicsFrame> recordingFrames;
        size_t playbackIndex = 0;
        bool isPlaying = false;
        
        // События
        std::unordered_map<std::string, 
            std::vector<std::pair<size_t, std::function<void(CollisionEvent*)>>>> collisionCallbacks;
        size_t nextCallbackId = 0;
        
        // Пул объектов
        std::vector<Rigidbody2D*> bodyPool;
        std::vector<Collider2D*> colliderPool;
        int poolSize = 100;
        
        // Регионы
        std::vector<PhysicsRegion> regions;
        
        // Приватные методы
        void integrateForces(float dt);
        void integrateVelocities(float dt);
        void updateColliders();
        void findNewContacts();
        void solveContacts(float dt);
        void solveVelocities(float dt);
        void solvePositions(float dt);
        void updateSleepState(float dt);
        void clearForces();
        
        void solveIsland(Island* island, float dt);
        void solveContact(Contact* contact, float dt);
        void solveJoint(Joint* joint, float dt);
        
        void processTasks();
        void addTask(std::function<void()> task);
        void waitForTasks();
        
        void updateStatistics();
        void drawDebugShapes();
        void drawContacts();
        void drawBroadphase();
        void drawAABBs();
        void drawNormals();
        void drawVelocities();
        void drawForces();
        
        void broadcastCollisionEvent(CollisionEvent* event);
        
        void saveFrame();
        void loadFrame(size_t index);
        
        Rigidbody2D* allocateBody();
        Collider2D* allocateCollider(ColliderType type);
        void deallocateBody(Rigidbody2D* body);
        void deallocateCollider(Collider2D* collider);
        
        void updateRegions();
        void addToRegion(Collider2D* collider);
        void removeFromRegion(Collider2D* collider);
        
        friend class Rigidbody2D;
        friend class Collider2D;
        friend class Broadphase;
        friend class CollisionSolver;
    };
}
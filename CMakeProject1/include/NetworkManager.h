#pragma once

#include <enet/enet.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/client.hpp>
#include <asio.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <functional>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

namespace CmakeProject1 {

    class NetworkManager {
    public:
        NetworkManager();
        ~NetworkManager();
        
        bool initialize();
        void shutdown();
        
        void update(float deltaTime);
        
        // Сервер
        bool startServer(int port = 7777, int maxClients = 32);
        void stopServer();
        bool isServerRunning() const { return serverRunning; }
        
        // Клиент
        bool connectToServer(const std::string& address, int port = 7777, 
                            int timeout = 5000);
        void disconnect();
        bool isConnected() const { return connected; }
        
        // Отправка сообщений
        void sendToServer(const std::string& messageType, const std::vector<uint8_t>& data, 
                         bool reliable = true);
        void sendToClient(ENetPeer* client, const std::string& messageType, 
                         const std::vector<uint8_t>& data, bool reliable = true);
        void sendToAll(const std::string& messageType, const std::vector<uint8_t>& data, 
                      bool reliable = true, ENetPeer* except = nullptr);
        
        // Сериализация
        template<typename T>
        void sendToServer(const std::string& messageType, const T& data, bool reliable = true) {
            std::vector<uint8_t> serialized;
            serializeData(data, serialized);
            sendToServer(messageType, serialized, reliable);
        }
        
        template<typename T>
        void sendToClient(ENetPeer* client, const std::string& messageType, 
                         const T& data, bool reliable = true) {
            std::vector<uint8_t> serialized;
            serializeData(data, serialized);
            sendToClient(client, messageType, serialized, reliable);
        }
        
        // RPC
        void registerRPC(const std::string& name, std::function<void(const std::vector<uint8_t>&)> handler);
        void callRPC(const std::string& name, const std::vector<uint8_t>& data, 
                    ENetPeer* target = nullptr, bool reliable = true);
        
        template<typename... Args>
        void callRPC(const std::string& name, Args&&... args) {
            std::vector<uint8_t> serialized;
            serializeTuple(std::make_tuple(std::forward<Args>(args)...), serialized);
            callRPC(name, serialized);
        }
        
        // Репликация
        class NetworkObject {
        public:
            uint32_t networkId = 0;
            GameObject* gameObject = nullptr;
            bool isOwner = false;
            bool isReplicated = false;
            float syncInterval = 0.1f;
            float lastSyncTime = 0.0f;
            
            std::vector<std::string> replicatedProperties;
            std::unordered_map<std::string, std::vector<uint8_t>> lastValues;
            
            void update(float deltaTime);
            void serialize(Json::Value& json) const;
            void deserialize(const Json::Value& json);
        };
        
        void registerNetworkObject(GameObject* gameObject, bool isOwner);
        void unregisterNetworkObject(GameObject* gameObject);
        NetworkObject* getNetworkObject(uint32_t networkId);
        
        // Лаг компенсация
        void enableLagCompensation(bool enable);
        void setInterpolation(bool enable);
        void setExtrapolation(bool enable);
        
        // Предсказание на стороне клиента
        void enableClientPrediction(bool enable);
        void enableServerReconciliation(bool enable);
        
        // Состояние
        struct NetworkStats {
            int bytesSent = 0;
            int bytesReceived = 0;
            int packetsSent = 0;
            int packetsReceived = 0;
            int packetLoss = 0;
            float ping = 0.0f;
            float jitter = 0.0f;
            float bandwidth = 0.0f;
        };
        
        NetworkStats getStats() const;
        NetworkStats getClientStats(ENetPeer* client) const;
        
        // События
        void setConnectCallback(std::function<void(ENetPeer*)> callback);
        void setDisconnectCallback(std::function<void(ENetPeer*)> callback);
        void setMessageCallback(std::function<void(ENetPeer*, const std::string&, 
                                                  const std::vector<uint8_t>&)> callback);
        
        // NAT Punchthrough
        void enableNATPunchthrough(bool enable);
        bool punchthroughToServer(const std::string& address, int port);
        
        // Вебсокеты
        void startWebSocketServer(int port = 8080);
        void stopWebSocketServer();
        void connectWebSocket(const std::string& url);
        
        // REST API
        void startRESTServer(int port = 8081);
        void stopRESTServer();
        void registerRESTEndpoint(const std::string& endpoint, 
                                 std::function<std::string(const std::string&)> handler);
        
        // Войс чат
        void startVoiceChat();
        void stopVoiceChat();
        void setVoiceChatVolume(float volume);
        
        // Матчмейкинг
        void findMatch(const std::string& gameMode, int minPlayers, int maxPlayers);
        void cancelMatchmaking();
        
        // Лобби
        void createLobby(const std::string& name, int maxPlayers, bool isPrivate = false);
        void joinLobby(const std::string& lobbyId);
        void leaveLobby();
        void setLobbyData(const std::string& key, const std::string& value);
        
        // P2P
        void enableP2P(bool enable);
        void connectP2P(const std::string& peerId);
        void disconnectP2P(const std::string& peerId);
        
        // Криптография
        void enableEncryption(bool enable);
        void setEncryptionKey(const std::string& key);
        
        // Компрессия
        void enableCompression(bool enable);
        void setCompressionLevel(int level);
        
        // QoS
        void setQualityOfService(bool enable, int bandwidth = 0, 
                               int latency = 0, int jitter = 0);
        
        // Регионы
        void setRegion(const std::string& region);
        std::string getBestRegion() const;
        
        // Релеи
        void addRelayServer(const std::string& address, int port);
        void removeRelayServer(const std::string& address);
        
        // Мониторинг
        void startMonitoring();
        void stopMonitoring();
        Json::Value getMonitoringData() const;
        
        // Запись/воспроизведение
        void startRecording();
        void stopRecording();
        void saveRecording(const std::string& path);
        void loadRecording(const std::string& path);
        void playRecording();
        
        // Тестирование
        void runNetworkTest();
        void benchmark(int packetSize = 1024, int packetCount = 1000);
        
        // Протоколы
        enum class Protocol {
            TCP,
            UDP,
            WebSocket,
            WebRTC,
            HTTP,
            HTTPS
        };
        
        void setProtocol(Protocol protocol);
        Protocol getProtocol() const { return currentProtocol; }
        
    private:
        // ENet
        ENetHost* serverHost = nullptr;
        ENetHost* clientHost = nullptr;
        ENetPeer* serverPeer = nullptr;
        
        bool serverRunning = false;
        bool connected = false;
        
        std::vector<ENetPeer*> clients;
        std::unordered_map<ENetPeer*, NetworkStats> clientStats;
        
        // Вебсокеты
        using WebSocketServer = websocketpp::server<websocketpp::config::asio>;
        using WebSocketClient = websocketpp::client<websocketpp::config::asio>;
        
        std::unique_ptr<WebSocketServer> webSocketServer;
        std::unique_ptr<WebSocketClient> webSocketClient;
        
        bool webSocketServerRunning = false;
        bool webSocketConnected = false;
        
        // REST
        std::unique_ptr<asio::io_context> restContext;
        std::unique_ptr<asio::ip::tcp::acceptor> restAcceptor;
        
        bool restServerRunning = false;
        
        // Сообщения
        struct NetworkMessage {
            std::string type;
            std::vector<uint8_t> data;
            ENetPeer* sender = nullptr;
            bool reliable = true;
            uint32_t sequence = 0;
            uint32_t ack = 0;
        };
        
        std::queue<NetworkMessage> incomingMessages;
        std::queue<NetworkMessage> outgoingMessages;
        std::mutex messageMutex;
        
        // RPC
        std::unordered_map<std::string, std::function<void(const std::vector<uint8_t>&)>> rpcHandlers;
        uint32_t nextRpcId = 1;
        
        // Объекты
        std::unordered_map<uint32_t, std::unique_ptr<NetworkObject>> networkObjects;
        std::unordered_map<GameObject*, NetworkObject*> gameObjectToNetworkObject;
        uint32_t nextNetworkId = 1;
        
        // Предсказание
        bool clientPrediction = false;
        bool serverReconciliation = false;
        std::vector<NetworkMessage> pendingMessages;
        
        // Лаг компенсация
        bool lagCompensation = false;
        bool interpolation = true;
        bool extrapolation = false;
        std::vector<NetworkMessage> historyBuffer;
        size_t historySize = 1000;
        
        // NAT Punchthrough
        bool natPunchthrough = false;
        std::string natServerAddress;
        int natServerPort = 0;
        
        // Войс чат
        bool voiceChatEnabled = false;
        std::vector<int16_t> voiceBuffer;
        
        // Матчмейкинг
        struct MatchmakingRequest {
            std::string gameMode;
            int minPlayers;
            int maxPlayers;
            std::chrono::steady_clock::time_point startTime;
        };
        
        std::unique_ptr<MatchmakingRequest> currentMatchmaking;
        
        // Лобби
        struct Lobby {
            std::string id;
            std::string name;
            int maxPlayers;
            bool isPrivate;
            std::vector<std::string> players;
            std::unordered_map<std::string, std::string> data;
        };
        
        std::unique_ptr<Lobby> currentLobby;
        std::unordered_map<std::string, Lobby> lobbies;
        
        // P2P
        bool p2pEnabled = false;
        std::unordered_map<std::string, ENetPeer*> p2pPeers;
        
        // Криптография
        bool encryptionEnabled = false;
        std::string encryptionKey;
        
        // Компрессия
        bool compressionEnabled = false;
        int compressionLevel = 6;
        
        // QoS
        bool qosEnabled = false;
        int qosBandwidth = 0;
        int qosLatency = 0;
        int qosJitter = 0;
        
        // Регионы
        std::string currentRegion = "auto";
        std::unordered_map<std::string, float> regionLatencies;
        
        // Релеи
        std::vector<std::pair<std::string, int>> relayServers;
        
        // Мониторинг
        bool monitoring = false;
        Json::Value monitoringData;
        
        // Запись
        bool recording = false;
        bool playing = false;
        std::vector<NetworkMessage> recordedMessages;
        size_t playbackIndex = 0;
        
        // Протокол
        Protocol currentProtocol = Protocol::UDP;
        
        // Статистика
        NetworkStats stats;
        std::chrono::high_resolution_clock::time_point lastStatUpdate;
        
        // Потоки
        std::thread networkThread;
        std::atomic<bool> networkRunning{false};
        std::mutex networkMutex;
        std::condition_variable networkCondition;
        
        // Таймеры
        std::chrono::steady_clock::time_point lastPingTime;
        float pingInterval = 1.0f;
        
        // События
        std::function<void(ENetPeer*)> connectCallback;
        std::function<void(ENetPeer*)> disconnectCallback;
        std::function<void(ENetPeer*, const std::string&, const std::vector<uint8_t>&)> messageCallback;
        
        // Приватные методы
        void networkThreadFunc();
        void processIncomingMessages();
        void processOutgoingMessages();
        void updateNetworkObjects(float deltaTime);
        void updateLagCompensation();
        void updateClientPrediction();
        void updateMatchmaking(float deltaTime);
        void updateLobbies(float deltaTime);
        void updateVoiceChat();
        void updateMonitoring();
        void updateRecording();
        void updateStatistics();
        
        void handleENetEvent(ENetEvent& event);
        void handleWebSocketMessage(websocketpp::connection_hdl hdl, 
                                   WebSocketServer::message_ptr msg);
        void handleRESTRequest(asio::ip::tcp::socket& socket, 
                              const std::string& request);
        
        void serializeData(const std::vector<uint8_t>& data, std::vector<uint8_t>& output);
        template<typename T>
        void serializeData(const T& data, std::vector<uint8_t>& output) {
            std::ostringstream oss(std::ios::binary);
            {
                cereal::BinaryOutputArchive archive(oss);
                archive(data);
            }
            std::string str = oss.str();
            output.assign(str.begin(), str.end());
        }
        
        template<typename Tuple, size_t... Is>
        void serializeTupleImpl(const Tuple& t, std::vector<uint8_t>& output, 
                               std::index_sequence<Is...>) {
            serializeData(std::make_tuple(std::get<Is>(t)...), output);
        }
        
        template<typename Tuple>
        void serializeTuple(const Tuple& t, std::vector<uint8_t>& output) {
            serializeTupleImpl(t, output, 
                              std::make_index_sequence<std::tuple_size<Tuple>::value>{});
        }
        
        template<typename T>
        T deserializeData(const std::vector<uint8_t>& data) {
            std::string str(data.begin(), data.end());
            std::istringstream iss(str, std::ios::binary);
            T result;
            {
                cereal::BinaryInputArchive archive(iss);
                archive(result);
            }
            return result;
        }
        
        void compressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
        void decompressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
        
        void encryptData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
        void decryptData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
        
        void calculatePing();
        void calculateJitter();
        void calculateBandwidth();
        
        void sendPing();
        void handlePong(ENetPeer* peer, const std::vector<uint8_t>& data);
        
        void replicateObject(NetworkObject* obj);
        void interpolateObject(NetworkObject* obj);
        void extrapolateObject(NetworkObject* obj);
        
        void reconcileClientState();
        void saveState(NetworkObject* obj);
        void restoreState(NetworkObject* obj, float time);
        
        void findBestRegion();
        void connectToBestRegion();
        
        void handleNATPunchthrough();
        void connectThroughRelay(const std::string& address, int port);
        
        void startWebRTC();
        void stopWebRTC();
        
        void handleError(const std::string& error);
        
        friend class NetworkObject;
    };
}
#pragma once
#include <string>
#include <vector>

namespace CmakeProject1 {

    enum class NetworkRole {
        Server,
        Client
    };

    struct NetworkMessage {
        int id;
        std::string data;
        std::string sender;
    };

    class NetworkManager {
    public:
        NetworkManager();
        ~NetworkManager();

        bool initialize(NetworkRole role, const std::string& address = "", int port = 0);
        void shutdown();

        bool connect(const std::string& address, int port);
        void disconnect();
        
        bool sendMessage(const NetworkMessage& message);
        std::vector<NetworkMessage> getMessages();
        
        bool isServer() const;
        bool isConnected() const;

        // Server-specific methods
        void startServer(int port);
        void stopServer();
        std::vector<std::string> getClientList() const;

    private:
        NetworkRole role;
        bool connected;
        std::string serverAddress;
        int serverPort;
    };

} // namespace CmakeProject1
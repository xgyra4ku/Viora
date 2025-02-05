#include "../inc/Server.h"
#include <functional> // для std::bind
#include "../inc/structMessage.h"

Server::Server() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server is listening on port 12345...\n";
}
Server::~Server() = default;

void Server::run() {
    while (true) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed\n";
                continue;
            }

            clients.push_back(clientSocket);
            std::thread(std::bind(&Server::HandleClient, this, clientSocket)).detach();
        }
}

void Server::HandleClient(const SOCKET clientSocket) {
    std::cout << "New client connected\n";

    while (true) {
        std::vector<char> buffer(1024);
        const int bytesReceived = recv(clientSocket, buffer.data(), buffer.size(), 0);
        if (bytesReceived <= 0) {
            std::cout << "Client disconnected\n";
            closesocket(clientSocket);
            return;//☻
        }
        sMessage b;

        buffer.resize(bytesReceived);
        b.deserialize(buffer);
        // message[bytesReceived] = '\0';
        std::cout << "Received: " << b.data["key"]<< "\n" << b.data["msg"] << std::endl;
        for (const auto& [key, value] : b.data) {
            std::cout << key << ": " << value << std::endl;
        }
        // Пересылка сообщения всем клиентам
        for (auto& client : clients) {
            // if (client != clientSocket) {
                send(client, buffer.data(), buffer.size(), 0);
            // }
        }
    }
}

void Server::stop() {
    closesocket(serverSocket);
    WSACleanup();
}

void Server::processMessages() {

}

void Server::connectDatabases() {

}

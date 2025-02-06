#include "../inc/Server.h"
#include <functional> // для std::bind
#include "../inc/structures.h"

#include <thread>
#include <sql.h>
#include <ws2tcpip.h>


Server::Server(int argc, char* argv[]) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cmd.printINFO("WSAStartup failed");
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cmd.printERROR("Socket creation failed");
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cmd.printERROR("Bind failed");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cmd.printERROR("Listen failed");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    cmd.printINFO("Server is listening on port 12345...");
}
Server::~Server() = default;

void Server::run() {
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cmd.printERROR("Accept failed");
            continue;
        }

        sClientInfo clientInfo;
        clientInfo.socket = clientSocket;
        clientInfo.username = "Unknown";
        clients.push_back(clientInfo);
        std::thread([this, clientSocket] { HandleClient(clientSocket); }).detach();
    }
}

void Server::HandleClient(const SOCKET clientSocket) {
    cmd.printINFO("Client connected");
    char bufferUsernameConnection[64];
    if (const int bytesReceived = recv(clientSocket, bufferUsernameConnection, sizeof(bufferUsernameConnection), 0); bytesReceived <= 0) {
        cmd.printINFO("Client disconnected");
        closesocket(clientSocket);
        return;
    }

    while (true) {
        std::vector<char> buffer(1024);
        const int bytesReceived = recv(clientSocket, buffer.data(), buffer.size(), 0);
        if (bytesReceived <= 0) {
            cmd.printINFO("Client disconnected");
            closesocket(clientSocket);
            return;
        }
        sMessage b;

        buffer.resize(bytesReceived);
        b.deserialize(buffer);
        // message[bytesReceived] = '\0';
        cmd.printINFO("Received: " + b.data["key"] + " " + b.data["msg"]);
        for (const auto& [key, value] : b.data) {
            cmd.printINFO(key + " : " + value);
        }
        // Пересылка сообщения всем клиентам
        for (auto& client : clients) {
            // if (client != clientSocket) {
                send(client.socket, buffer.data(), buffer.size(), 0);
            // }
        }
    }
}

void Server::stop() {
    closesocket(serverSocket);
    WSACleanup();
}

void Server::processMessages(std::map<std::string, std::string>& data) {
    if (data["username"] != "") {
        for (auto& client : clients) {
            if (client.username == data["username"]) {
                client.username = data["newUsername"];
            }
        }
    }
}

void Server::connectDatabases() {

}

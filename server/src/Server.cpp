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
        clients.push_back(clientInfo);
        std::thread([this, clientSocket] { HandleClient(clientSocket); }).detach();
    }
}

void Server::HandleClient(const SOCKET clientSocket) {
    cmd.printINFO("Client connected");
    std::vector<char> bufferVerification(1024);
    const int bytesReceived = recv(clientSocket, bufferVerification.data(), bufferVerification.size(), 0);
    if (bytesReceived <= 0) {
        cmd.printINFO("Client disconnected");
        closesocket(clientSocket);
        return;
    }
    bufferVerification.resize(bytesReceived);
    sMessage msgVerification;
    msgVerification.deserialize(bufferVerification);
    if (msgVerification.data["verification"] == "yes") {
        //if (msgVerification.data["hash"] == ...) { // запрос хеша из базы данных
        //}
    } else if (msgVerification.data["verification"] == "no") {
        // if (msgVerification.data["username"] == "admin" && msgVerification.data["password"] == "admin") { проверяем
        //     sMessage msg;
        //     msg.type = 1;
        //     msg.data["key"] = "verification";
        //     msg.data["msg"] = "yes";
        //     std::vector<char> buffer = msg.serialize();
        // }
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
    if (!data["username"].empty()) {

    }
}

void Server::connectDatabases() {

}


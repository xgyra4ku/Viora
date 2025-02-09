#include "../inc/Server.h"
#include <functional> // для std::bind
#include "../inc/structures.h"

#include <thread>
#include <ws2tcpip.h>
#include <iostream>

Server::Server(int argc, char* argv[])
    : conn()
{
    processArguments(argc, argv);
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cmd.printINFO("WSAStartup failed");
        exit(1);
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cmd.printERROR("Socket creation failed");
        WSACleanup();
        exit(2);
    }

    serverAddr.sin_family = AF_INET;
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cmd.printERROR("Bind failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(3);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cmd.printERROR("Listen failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(4);
    }

    cmd.printINFO("Server is listening on " + address + ":" + port);
}
Server::~Server() = default;

void Server::processArguments(int argc, char *argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    if (searchArgumentInVector(args, "--help") || argc == 1) {
        std::cout << "Usage: server [options]\n"
        << "Options:\n"
        << "--help: Show this help message\n"
        << "--version: Show this version server\n"
        << "--port <port>: Set the port number (default: 8080)\n"
        << "--address <address>: Set the server address (default: 0.0.0.0)\n"
        << "--debug: Enable debug mode (default: false)\n"
        << "--db <address> <port> <user> <password> <database>: Connect to the database (default: 127.0.0.1 5432 postgres postgres postgres)\n"
        << "--testdb: Connect to the test database"
        << std::endl;
        exit(0);
    }
    if (searchArgumentInVector(args, "--version")) {
        std::cout << "Version: " << SERVER_VERSION << std::endl;
        exit(0);
    }
    if (searchArgumentInVector(args, "--port")) {
        const int num = getArgumentValue(args, "--port");
        port = num < args.size() ? args[num+1] : "";
        if (port.empty()) {
            cmd.printERROR("Invalid port number");
            exit (1);
        }
        // нужно добавить проверку правильного ввода порта
        serverAddr.sin_port = htons(std::stoi(port));
    }
    else {
        serverAddr.sin_port = htons(12345);
        port = "12345";
    }

    if (searchArgumentInVector(args, "--address")) {
        const int num = getArgumentValue(args, "--address");
        address = num < args.size() ? args[num+1] : "";
        if (address.empty()) {
            cmd.printERROR("Invalid address");
            exit (1);
        }
        // нужно добавить проверку правильного ввода адреса
        inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr);
    } else {
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        address = "0.0.0.0";
    }
    if (searchArgumentInVector(args, "--debug")) {
        DEBUG = true;
    }
    if (searchArgumentInVector(args, "--db")) {
        int db = getArgumentValue(args, "--db");
        if (db+5 > args.size()) {
            cmd.printERROR("Invalid database connection string");
            exit (1);
        }
        connectionInfo = (std::string("host=") + args[db+1]
            + " port=" + args[db+2]
            + " user=" + args[db+3]
            + " password=" + args[db+4]
            + " dbname=" + args[db+5]).c_str();
    }
    if (searchArgumentInVector(args, "--testdb")) {
        connectDatabases();
        disconnectDatabases();
        exit(0);
    }
}


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
    try {
        conn = PQconnectdb(CONN_INFO_DEFAULT);
        if (PQstatus(conn) == CONNECTION_OK) {
            cmd.printINFO("Connection successful! Database: ", false);
            std::cout << PQdb(conn) << std::endl;
        } else {
            cmd.printERROR("Connection failed!");
        }
    } catch (const std::exception &e) {
        cmd.printERROR(e.what());
    }
}

void Server::disconnectDatabases() {
    if (PQstatus(conn) == CONNECTION_OK) {
        PQfinish(conn);
        cmd.printINFO("Disconnection successful! Database: ", false);
        std::cout << PQdb(conn) << std::endl;
    } else {
        cmd.printERROR("Disconnection failed!");
    }
}

bool Server::searchArgumentInVector(const std::vector<std::string> &vector, const std::string &argument) {
    return std::find(vector.begin(), vector.end(), argument) != vector.end();
}

int Server::getArgumentValue(const std::vector<std::string> &vector, const std::string &argument) {
    if (const auto it = std::find(vector.begin(), vector.end(), argument); it != vector.end()) {
        return static_cast<int>(std::distance(vector.begin(), it));
    }
    return -1;
}





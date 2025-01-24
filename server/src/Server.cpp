#include "../inc/Server.h"
Server::Server() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Инициализация masterSet
    FD_ZERO(&masterSet);
    FD_SET(serverSocket, &masterSet);

    std::cout << "Server is listening on port " << SERVER_PORT << "..." << std::endl;
}
Server::~Server() {
    for (const auto& [socket, _] : clientData) {
        closesocket(socket);
    }
        closesocket(serverSocket);
    WSACleanup();
}


void Server::run() {
    while (true) {
        fd_set readSet = masterSet;

        // Ожидание активности на одном из сокетов
        int socketCount = select(0, &readSet, nullptr, nullptr, nullptr);
        if (socketCount == SOCKET_ERROR) {
            std::cerr << "Select failed: " << WSAGetLastError() << std::endl;
            break;
        }

        // Обходим все сокеты в наборе
        for (int i = 0; i < readSet.fd_count; ++i) {
            SOCKET sock = readSet.fd_array[i];

            // Новый входящий запрос
            if (sock == serverSocket) {
                handleNewConnection();
            } else {
                // Сообщение от существующего клиента
                handleClientMessage(sock);
            }
        }
    }
}

void Server::handleNewConnection() {
    sockaddr_in clientAddr{};
    int clientAddrSize = sizeof(clientAddr);

    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        return;
    }

    // Добавляем новый сокет в набор
    FD_SET(clientSocket, &masterSet);

    // Сохраняем клиента в список
    clientData[clientSocket] = "";

    std::cout << "New client connected: " << clientSocket << std::endl;
    sendMessageToClient(clientSocket, "Welcome to the server!");
}

void Server::handleClientMessage(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);

    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived <= 0) {
        // Клиент отключился
        std::cout << "Client disconnected: " << clientSocket << std::endl;

        FD_CLR(clientSocket, &masterSet);  // Удаляем из набора
        closesocket(clientSocket);        // Закрываем сокет
        clientData.erase(clientSocket);   // Удаляем из списка клиентов
        return;
    }

    std::string message(buffer);
    std::cout << "Received from client " << clientSocket << ": " << message << std::endl;

    // Пример логики: пересылка сообщения всем клиентам
    for (const auto& [otherSocket, _] : clientData) {
        // if (otherSocket != clientSocket) {
            sendMessageToClient(otherSocket, "Client " + std::to_string(clientSocket) + ": " + message);
        // }
    }
}

void Server::sendMessageToClient(SOCKET clientSocket, const std::string& message) {
    send(clientSocket, message.c_str(), message.size(), 0);
}
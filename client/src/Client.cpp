#include "../inc/Client.h"

Client::Client() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Создание сокета
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);
}

Client::~Client() {
    closesocket(clientSocket);
    WSACleanup();
}

void Client::run() {
    // Подключение к серверу
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Connected to the server!" << std::endl;

    // Создаем два набора сокетов для использования с select
    fd_set readSet;
    FD_ZERO(&readSet);


    char buffer[BUFFER_SIZE];
    while (true) {
        // Добавляем сокет клиента и стандартный ввод (stdin) в набор
        FD_SET(clientSocket, &readSet);
        FD_SET(0, &readSet);  // 0 - дескриптор стандартного ввода
        // Если доступны данные от сервера
        if (FD_ISSET(clientSocket, &readSet)) {
            std::memset(buffer, 0, BUFFER_SIZE);
            int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (bytesReceived > 0) {
                std::cout << "\nServer response: " << buffer << std::endl;
            } else if (bytesReceived == 0) {
                std::cout << "Server disconnected." << std::endl;
                break;
            } else {
                std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
                break;
            }
        }

        // // Определяем, какой сокет готов
        // int socketCount = select(0, &readSet, nullptr, nullptr, nullptr);
        // if (socketCount == SOCKET_ERROR) {
        //     std::cerr << "Select failed: " << WSAGetLastError() << std::endl;
        //     break;
        // }

        // Если доступен ввод с клавиатуры
        if (FD_ISSET(0, &readSet)) {
            std::cout << "Enter message to send (or 'exit' to quit): ";
            std::string message;
            std::getline(std::cin, message);

            if (message == "exit") {
                break;
            }

            // Отправка сообщения на сервер
            if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
                break;
            }
        }

    }
}

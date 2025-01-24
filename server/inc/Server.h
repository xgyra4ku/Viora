#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <unordered_map>

#pragma comment(lib, "ws2_32.lib")

const int SERVER_PORT = 8080;
const int BUFFER_SIZE = 1024;

class Server {
    sockaddr_in serverAddr{}, clientAddr{};
    SOCKET serverSocket{}, clientSocket{};
    char buffer[BUFFER_SIZE]{};
    int clientAddrSize{};
    fd_set masterSet;   // Главный набор сокетов
    std::unordered_map<SOCKET, std::string> clientData;  // Соответствие сокетов и их данных

    void handleNewConnection();
    void handleClientMessage(SOCKET clientSocket);
    void sendMessageToClient(SOCKET clientSocket, const std::string& message);
public:
    Server();
    ~Server();
    void run();
};



#endif //SERVER_H

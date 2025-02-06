#include "inc/server.h"

int main(int argc, char* argv[]) {
    Server server;
    server.run();
    return 0;
}

// #include <iostream>
// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <vector>
// #include <thread>
//
// #pragma comment(lib, "ws2_32.lib")
//
// std::vector<SOCKET> clients;
//
// void HandleClient(SOCKET clientSocket) {
//     char buffer[1024];
//     while (true) {
//         int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//         if (bytesReceived <= 0) {
//             std::cout << "Client disconnected\n";
//             closesocket(clientSocket);
//             return;
//         }
//         buffer[bytesReceived] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
//
//         // Пересылка сообщения всем клиентам
//         for (auto& client : clients) {
//             if (client != clientSocket) {
//                 send(client, buffer, bytesReceived, 0);
//             }
//         }
//     }
// }
//
// int main() {
//     WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         std::cerr << "WSAStartup failed\n";
//         return 1;
//     }
//
//     SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == INVALID_SOCKET) {
//         std::cerr << "Socket creation failed\n";
//         WSACleanup();
//         return 1;
//     }
//
//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(12345);
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//
//     if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//         std::cerr << "Bind failed\n";
//         closesocket(serverSocket);
//         WSACleanup();
//         return 1;
//     }
//
//     if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
//         std::cerr << "Listen failed\n";
//         closesocket(serverSocket);
//         WSACleanup();
//         return 1;
//     }
//
//     std::cout << "Server is listening on port 12345...\n";
//
//     while (true) {
//         SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
//         if (clientSocket == INVALID_SOCKET) {
//             std::cerr << "Accept failed\n";
//             continue;
//         }
//
//         clients.push_back(clientSocket);
//         std::thread(HandleClient, clientSocket).detach();
//     }
//
//     closesocket(serverSocket);
//     WSACleanup();
//     return 0;
// }


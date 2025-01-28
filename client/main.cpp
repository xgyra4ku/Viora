#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <map>

#pragma comment(lib, "ws2_32.lib")
struct sMessage {
    int type{};
    std::map<std::string, std::string> data;
};
sMessage message;
sMessage message1;
void ReceiveMessages(SOCKET socket) {
    while (true) {
        int bytesReceived = recv(socket, (char*)&message, sizeof(message), 0);
        if (bytesReceived <= 0) {
            std::cout << "Disconnected from server\n";
            break;
        }
        //buffer[bytesReceived] = '\0';
        std::cout << "Received: " << message.type << " " << message1.data["key"] << std::endl;
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server\n";

    std::thread(ReceiveMessages, clientSocket).detach();

    std::string message;
    message1.type = 1;
    message1.data["key"] = "value";
    while (true) {
        std::getline(std::cin, message);
        send(clientSocket, (char*)&message1, sizeof(message1), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
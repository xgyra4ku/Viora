#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>

#include >

#pragma comment(lib, "ws2_32.lib")

const int SERVER_PORT = 8080;
const int BUFFER_SIZE = 1024;

class Server {
    std::vector<SOCKET> clients;
    sockaddr_in serverAddr{}, clientAddr{};
    SOCKET serverSocket{}, clientSocket{};
    int clientAddrSize{};

    void HandleClient(SOCKET clientSocket);
public:
    Server();
    ~Server();
    void run();
};



#endif //SERVER_H

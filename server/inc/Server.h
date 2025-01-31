#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>

#include "structMessage.h"

#pragma comment(lib, "ws2_32.lib")

constexpr int SERVER_PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

class Server {
    sMessage message;
    std::vector<SOCKET> clients;
    sockaddr_in serverAddr{}, clientAddr{};
    SOCKET serverSocket{}, clientSocket{};
    int clientAddrSize{};

    void HandleClient(SOCKET clientSocket);
public:
    /**
     * Default constructor.
     * Initializes the server socket and addresses.
     */
    Server();
    /**
     * Destructor for the Server class.
     * Cleans up resources by closing sockets and performing necessary cleanup.
     */
    ~Server();

    void run();
};



#endif //SERVER_H


#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class Client {
    SOCKET clientSocket{};
    sockaddr_in serverAddr{};
    const char* SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    const int BUFFER_SIZE = 1024;
public:
    Client();
    ~Client();
    void run();
};



#endif //CLIENT_H

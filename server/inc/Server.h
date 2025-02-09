#ifndef SERVER_H
#define SERVER_H

#define SERVER_VERSION "0.0.2"
#define CONN_INFO_DEFAULT "host=localhost port=5555 dbname=testdb user=kyra password=1029 connect_timeout=10"

#include <winsock2.h>
#include <vector>
#include <libpq-fe.h>

#include "structures.h"
#include "Info.h"

#pragma comment(lib, "ws2_32.lib")

constexpr int SERVER_PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

class Server {
    bool DEBUG = false;


    PGconn *conn;

    Info cmd;

    sMessage message;

    std::vector<sClientInfo> clients;

    sockaddr_in serverAddr{}, clientAddr{};
    SOCKET serverSocket{}, clientSocket{};

    int clientAddrSize{};

    std::string address, port;
    const char *connectionInfo = CONN_INFO_DEFAULT;


    void HandleClient(SOCKET clientSocket);
    void connectDatabases();
    void disconnectDatabases();
    void processMessages(std::map<std::string, std::string> &data);
    void processArguments(int argc, char* argv[]);

    static bool searchArgumentInVector(const std::vector<std::string> &vector, const std::string &argument);
    static int getArgumentValue(const std::vector<std::string> &vector, const std::string &argument);
public:
    /**
     * Default constructor.
     * Initializes the server socket and addresses.
     */
    Server(int argc, char* argv[]);
    /**
     * Destructor for the Server class.
     * Cleans up resources by closing sockets and performing necessary cleanup.
     */
    ~Server();

    void run();
    void stop();
};



#endif //SERVER_H

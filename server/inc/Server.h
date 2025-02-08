#ifndef SERVER_H
#define SERVER_H

#define SERVER_VERSION "0.0.2"

#include <winsock2.h>
#include <vector>
#include <pqxx/pqxx>

#include "structures.h"
#include "Info.h"

#pragma comment(lib, "ws2_32.lib")

constexpr int SERVER_PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

class Server {
private:
    pqxx::connection* conn;
    pqxx::work* txn;
    pqxx::nontransaction* ntxn;

    Info cmd;

    sMessage message;

    std::vector<sClientInfo> clients;

    sockaddr_in serverAddr{}, clientAddr{};
    SOCKET serverSocket{}, clientSocket{};

    int clientAddrSize{};

    void HandleClient(SOCKET clientSocket);
    void connectDatabases();
    void disconnectDatabases() const;
    void processMessages(std::map<std::string, std::string> &data);

    static bool searchArgumentInVector(const std::vector<std::string> &vector, const std::string &argument);
    static std::string getArgumentValue(const std::vector<std::string> &vector, const std::string &argument);
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

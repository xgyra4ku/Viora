/**
 *  Server
 *  @version 0.0.21
 *  @date 01.01.2025
 *  @file Server.cpp
 *  @brief Файл реализации класса Server
 */
/**
 * Local includes
 */
#include "../inc/Server.h"
#include "../inc/structures.h"

/**
 * Includes
 */
// для работы потоками
#include <thread>
// для работы с сокетами
#include <ws2tcpip.h>
// для работы с векторами
#include <iostream>
// для работы со строками
#include <string>
// для работы с функциями
#include <functional>

/**
 * @brief Конструктор класса Server
 * @param argc - количество аргументов
 * @param argv - аргументы
 */
Server::Server(const int argc, char* argv[])
    : conn()
    , serverAddr()
    , clientAddr()
    , serverSocket()
    , clientSocket()
    , clientAddrSize()
{
    // Парсинг аргументов
    processArguments(argc, argv);
    // Инициализация сокета
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cmd.printINFO("WSAStartup failed");
        exit(1);
    }
    // Создание сокета
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cmd.printERROR("Socket creation failed");
        WSACleanup();
        exit(2);
    }
    // Настройка сокета
    serverAddr.sin_family = AF_INET;
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cmd.printERROR("Bind failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(3);
    }
    // Начало прослушивания
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cmd.printERROR("Listen failed");
        closesocket(serverSocket);
        WSACleanup();
        exit(4);
    }
    // Вывод информации
    cmd.printINFO("Server is listening on " + address + ":" + port);
}

/**
 * @brief Деструктор класса Server
 */
Server::~Server() {
    // Остановка сервера и закрытие соединения
    disconnectDatabases();
    stop();
}

/**
  * @brief processArguments - обработка аргументов
  * @param argc - количество аргументов
  * @param argv - аргументы
  */
void Server::processArguments(int argc, char *argv[]) {
    // Парсинг аргументов в вектор для удобства
    std::vector<std::string> args(argv + 1, argv + argc);

    // Обработка аргумента --help или когда нет аргументов
    if (searchArgumentInVector(args, "--help") || argc == 1) {
        std::cout << "Usage: server [options]\n"
        << "Options:\n"
        << "--help: Show this help message\n"
        << "--version: Show this version server\n"
        << "--port <port>: Set the port number (default: 8080)\n"
        << "--address <address>: Set the server address (default: 0.0.0.0)\n"
        << "--debug: Enable debug mode (default: false)\n"
        << "--db <address> <port> <user> <password> <database>: Connect to the database (default: 127.0.0.1 5432 postgres postgres postgres)\n"
        << "--testdb: Connect to the test database"
        << std::endl;
        exit(0);
    }

    // Обработка аргумента --version
    if (searchArgumentInVector(args, "--version")) {
        std::cout << "Version: " << SERVER_VERSION << std::endl;
        exit(0);
    }

    // Обработка аргумента --port
    if (searchArgumentInVector(args, "--port")) {
        const int num = getArgumentValue(args, "--port");
        port = num < args.size() ? args[num+1] : ""; // получаем значение порта
        if (port.empty()) {
            cmd.printERROR("Invalid port number");
            exit (1);
        }
        // нужно добавить проверку правильного ввода порта
        serverAddr.sin_port = htons(std::stoi(port));
    }
    else { // по умолчанию
        serverAddr.sin_port = htons(12345);
        port = "12345";
    }

    // Обработка аргумента --address
    if (searchArgumentInVector(args, "--address")) {
        const int num = getArgumentValue(args, "--address");
        address = num < args.size() ? args[num+1] : "";
        if (address.empty()) { // получаем значение адреса
            cmd.printERROR("Invalid address");
            exit (1);
        }
        // нужно добавить проверку правильного ввода адреса
        inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr);
    } else { // по умолчанию
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        address = "0.0.0.0";
    }

    // Обработка аргумента --debug
    if (searchArgumentInVector(args, "--debug")) {
        DEBUG = true;
    }

    // Обработка аргумента --db
    if (searchArgumentInVector(args, "--db")) {
        int db = getArgumentValue(args, "--db");
        if (db+5 > args.size()) { // проверка на количество аргументов
            cmd.printERROR("Invalid database connection string");
            exit (1);
        }
        // нужно добавить проверку правильного ввода строки подключения
        // создаем строку подключения
        connectionInfo = (std::string("host=") + args[db+1]
            + " port=" + args[db+2]
            + " user=" + args[db+3]
            + " password=" + args[db+4]
            + " dbname=" + args[db+5]).c_str();
    }

    // Обработка аргумента --testdb
    if (searchArgumentInVector(args, "--testdb")) {
        connectDatabases();
        disconnectDatabases();
        exit(0);
    }
}

/**
 * @brief Run - запуск сервера
 */
void Server::run() {
    // Цикл прослушивания
    while (true) {
        // Принятие соединения
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cmd.printERROR("Accept failed");
            continue;
        }
        // создание информации о клиенте
        sClientInfo clientInfo;
        clientInfo.socket = clientSocket;
        clients.push_back(clientInfo);
        std::thread([this, clientSocket] { HandleClient(clientSocket); }).detach();// запуск потока
    }
}

void Server::verificationClient(const SOCKET &clientSocket) {
    std::vector<char> bufferVerification(1024);
    const int bytesReceived = recv(clientSocket, bufferVerification.data(), bufferVerification.size(), 0);
    if (bytesReceived <= 0) {
        cmd.printINFO("Client disconnected");
        closesocket(clientSocket);
        return;
    }

    bufferVerification.resize(bytesReceived);
    sMessage msgVerification;
    msgVerification.deserialize(bufferVerification);
    if (msgVerification.data["verification"] == "yes") {
        //if (msgVerification.data["hash"] == ...) { // запрос хеша из базы данных
        //}
    } else if (msgVerification.data["verification"] == "no") {
        // if (msgVerification.data["username"] == "admin" && msgVerification.data["password"] == "admin") { проверяем
        //     sMessage msg;
        //     msg.type = 1;
        //     msg.data["key"] = "verification";
        //     msg.data["msg"] = "yes";
        //     std::vector<char> buffer = msg.serialize();
        // }
    } 
}


/**
  * @brief HandleClient - это функция обработки клиента
  * @param clientSocket - сокет клиента
  */
void Server::HandleClient(const SOCKET clientSocket) {
    cmd.printINFO("Client connected");

    while (true) {
        std::vector<char> buffer(1024);
        const int bytesReceived = recv(clientSocket, buffer.data(), buffer.size(), 0);
        if (bytesReceived <= 0) {
            cmd.printINFO("Client disconnected");
            closesocket(clientSocket);
            return;
        }
        sMessage b;

        buffer.resize(bytesReceived);
        b.deserialize(buffer);
        // message[bytesReceived] = '\0';
        cmd.printINFO("Received: " + b.data["key"] + " " + b.data["msg"]);
        for (const auto& [key, value] : b.data) {
            cmd.printINFO(key + " : " + value);
        }
        // Пересылка сообщения всем клиентам
        for (auto& client : clients) {
            // if (client != clientSocket) {
                send(client.socket, buffer.data(), buffer.size(), 0);
            // }
        }
    }
}


/**
  * @brief stop - остановка сервера
  */
void Server::stop() {
    closesocket(serverSocket);
    WSACleanup();
}

/**
  * @brief processMessages - обработка сообщений
  * @param data - данные которые пришли
  */
void Server::processMessages(std::map<std::string, std::string>& data) {
    if (!data["username"].empty()) {

    }
}

/**
  * @brief connectDatabases - подключение к базам
  */
void Server::connectDatabases() {
    try {
        std::cout << connectionInfo << std::endl;
        conn = PQconnectdb(connectionInfo);
        if (PQstatus(conn) == CONNECTION_OK) {
            cmd.printINFO("Connection successful! Database: ", false);
            std::cout << PQdb(conn) << std::endl;
        } else {
            cmd.printERROR("Connection failed!");
        }
    } catch (const std::exception &e) {
        cmd.printERROR(e.what());
    }
}

/**
  * @brief disconnectDatabases - отключение от баз
  */
void Server::disconnectDatabases() {
    if (PQstatus(conn) == CONNECTION_OK) {
        PQfinish(conn);
        cmd.printINFO("Disconnection successful! Database: ", false);
        std::cout << PQdb(conn) << std::endl;
    } else {
        cmd.printERROR("Disconnection failed!");
    }
}

/**
 * @brief searchArgumentInVector - поиск аргумента в векторе
 * @param vector - вектор с аргументами
 * @param argument - аргумент
 * @return если аргумент найден возвращает true если не найден возвращает false
 */
bool Server::searchArgumentInVector(const std::vector<std::string> &vector, const std::string &argument) {
    return std::find(vector.begin(), vector.end(), argument) != vector.end();
}

/**
  * @brief getArgumentValue - получение значения аргумента
  * @param vector - вектор с аргументами
  * @param argument - аргумент
  * @return если аргумент найден возвращает его индекс если не найден возвращает -1
  */
int Server::getArgumentValue(const std::vector<std::string> &vector, const std::string &argument) {
    if (const auto it = std::find(vector.begin(), vector.end(), argument); it != vector.end()) {
        return static_cast<int>(std::distance(vector.begin(), it));
    }
    return -1;
}





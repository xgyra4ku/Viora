/**
 *  Server
 *  @version 0.0.21
 *  @date 01.01.2025
 *  @file Server.h
 */
#ifndef SERVER_H
#define SERVER_H

/**
 * Server version
 */
#define SERVER_VERSION "0.0.21"
/**
 * Default connection info
 */
#define CONN_INFO_DEFAULT "host=localhost port=5432 dbname=testdb user=postgres password=postgres connect_timeout=10"

/**
 * Includes
 */
// windsock2.h - для работы с сокетами
#include <winsock2.h>
// vector - для работы с векторами
#include <vector>
// lib-fe.h - для работы с базою̆ данных postgresql
#include <libpq-fe.h>

/**
 * Local includes
 */
// structures.h - для работы с структурами
#include "structures.h"
// Info.h - для работы с информациею̆ и выводом
#include "Info.h"

/**
 * Libs
 */
#pragma comment(lib, "ws2_32.lib")

/**
 * Constants
 */
constexpr int SERVER_PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

/**
 * Main server class
 */
class Server {
 /**
  * Constants
  */
 const char *SERVER_ADDRESS = "127.0.0.1";
 const char *connectionInfo = CONN_INFO_DEFAULT;

 /**
  * Flags
  */
 bool DEBUG = false;

 /**
  * Objects and variables
  */
 //Databases
 PGconn *conn;
 std::string address, port;
 //Info
 Info cmd;
 //Network
 sMessage message;
 sockaddr_in serverAddr, clientAddr;
 SOCKET serverSocket, clientSocket;
 int clientAddrSize;

 /**
  * Vectors
  */
 std::vector<sClientInfo> clients;
 /**
  * Other variables
  */

 /**
  * Private functions
  */

 /**
  * @brief HandleClient - это функция обработки клиента
  * @param clientSocket - сокет клиента
  */
 void HandleClient(SOCKET clientSocket);

 /**
  * @brief verificationClient - функция верификации клиента
  * @param clientSocket - сокет клиента
  */
 void verificationClient(const SOCKET& clientSocket);

 /**
  * @brief processMessages - обработка сообщений
  * @param data - данные которые пришли
  */
 void processMessages(std::map<std::string, std::string> &data);

 /**
  * @brief connectDatabases - подключение к базам
  */
 void connectDatabases();

 /**
  * @brief disconnectDatabases - отключение от баз
  */
 void disconnectDatabases();

 /**
  * @brief processArguments - обработка аргументов
  * @param argc - количество аргументов
  * @param argv - аргументы
  */
 void processArguments(int argc, char *argv[]);

 /**
  * Private static functions
  */

 /**
  * @brief searchArgumentInVector - поиск аргумента в векторе
  * @param vector - вектор с аргументами
  * @param argument - аргумент
  * @return если аргумент найден возвращает true если не найден возвращает false
  */
 static bool searchArgumentInVector(const std::vector<std::string> &vector, const std::string &argument);

 /**
  * @brief getArgumentValue - получение значения аргумента
  * @param vector - вектор с аргументами
  * @param argument - аргумент
  * @return если аргумент найден возвращает его индекс если не найден возвращает -1
  */
 static int getArgumentValue(const std::vector<std::string> &vector, const std::string &argument);

public:
 /**
  * @brief Конструктор класса Server
  * @param argc - количество аргументов
  * @param argv - аргументы
  */
 Server(int argc, char *argv[]);

 /**
  * @brief Деструктор
  */
 ~Server();

 /**
  * @brief run - запуск сервера
  */
 void run();

 /**
  * @brief stop - остановка сервера
  */
 void stop();
};

/**
 * End
 */
#endif //SERVER_H
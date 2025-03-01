#include "../inc/Info.h"

#include <iomanip>
#include <iostream>
#include <chrono>
#include <windows.h>


Info::Info() {
    updateTime();
    enableANSI();
}

Info::~Info() {
    delete localTime;
}
//
// Вывод ошибки
//
void Info::printERROR(const std::string& strValue, const bool lineBreak) {
    updateTime();
    std::cout << RED << '[' << std::put_time(localTime, "%H:%M:%S") << "][" << "ERROR]: " << strValue << RESET;
    if (lineBreak) std::cout << std::endl;
}

//
// Вывод предупреждения
//
void Info::printWARNING(const std::string& strValue, const bool lineBreak) {
    updateTime();
    std::cout << RESET << '[' << std::put_time(localTime, "%H:%M:%S") << "][" << "WARNING]: " << strValue << RESET;
    if (lineBreak) std::cout << std::endl;
}

//
// Вывод информации
//
void Info::printINFO(const std::string& strValue, const bool lineBreak) {
    updateTime();
    std::cout << GREEN << '[' << std::put_time(localTime, "%H:%M:%S") << "][" << "INFO]: " << strValue << RESET;
    if (lineBreak) std::cout << std::endl;
}

//
// Обновления времени
//
void Info::updateTime() {
    // Получаем текущее время
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Преобразуем в строку
    localTime = std::localtime(&now_time);
}

//
// Для окрашивания консоли
//
void Info::enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
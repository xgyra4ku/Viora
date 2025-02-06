#ifndef INFO_H
#define INFO_H

#include <string>
#include <iomanip>

// linux
// #define red "\x1B[31m"
// #define green "\x1B[32m"
// #define yellow "\x1B[33m"
// #define reset "\x1B[0m"

// windows
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"


class Info {
public:
    Info();
    ~Info();

    void printINFO(const std::string &strValue, bool lineBreak = true);
    void printWARNING(const std::string &strValue, bool lineBreak = true);
    void printERROR(const std::string &strValue, bool lineBreak = true);

private:
    void enableANSI();
    void updateTime();
    std::tm* localTime;
};

#endif //INFO_H

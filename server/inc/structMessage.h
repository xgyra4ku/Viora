#ifndef STRUCTMESSAGE_H
#define STRUCTMESSAGE_H
#include <map>
#include <string>

struct message {
    int type;
    std::map<std::string, std::string> data;
};

#endif //STRUCTMESSAGE_H

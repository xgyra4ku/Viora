#ifndef INTERFACECHATMANAGER_H
#define INTERFACECHATMANAGER_H

#include <string>

class InterfaceChat {
private:
    std::string nameFolder;
    unsigned int idChat = 0;
protected:

public:
    virtual ~InterfaceChat() = default;

    void setNameFolder(const std::string& name);
    void setIDChat(unsigned int id);
};



#endif //INTERFACECHATMANAGER_H

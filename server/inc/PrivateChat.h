#ifndef PRIVATECHATMANAGER_H
#define PRIVATECHATMANAGER_H

#include "InterfaceChat.h"

class PrivateChat final : protected InterfaceChat {
public:
    using InterfaceChat::setNameFolder;
    using InterfaceChat::setIDChat;
};

#endif //PRIVATECHATMANAGER_H

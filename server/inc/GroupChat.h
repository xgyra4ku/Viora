#ifndef GROUPCHATMANAGER_H
#define GROUPCHATMANAGER_H

#include "InterfaceChat.h"

class GroupChat final : protected InterfaceChat {
public:
    using InterfaceChat::setNameFolder;
    using InterfaceChat::setIDChat;
};



#endif //GROUPCHATMANAGER_H

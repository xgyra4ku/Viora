#ifndef CHATSYSTEM_H
#define CHATSYSTEM_H

#include <filesystem>
#include <string>

#include "PrivateChat.h"
#include "GroupChat.h"

namespace fs = std::filesystem;

class ChatSystem {
    const fs::path* ROOT_DIR;

    bool idCheck(unsigned int id);
public:
    ChatSystem();
    ~ChatSystem();

    PrivateChat createPrivateChat(const unsigned int& userID1, const unsigned int& userID2);
    // GroupChat createGroupChat(const unsigned int& userID, const std::string& groupName);
};


#endif //CHATSYSTEM_H

/*
 *📁Filesystem
 *
 *
 *  📁 chat_data/
 *   ├── 📁 users/
 *   │    └── 📁 {user_id}/
 *   │         ├── 📄 profile.json      // Данные пользователя
 *   │         └── 📄 avatar.jpg        // или ссылка на файл в CDN
 *   │
 *   ├── 📁 private_chats/
 *   │    └── 📁 {chat_id}/             // Уникальный ID чата (напр., user1_user2 или хеш)
 *   │         ├── 📄 metadata.json     // Инфо о чате (участники, дата создания)
 *   │         └── 📁 messages/
 *   │              ├── 📄 batch_1.json // Сообщения 1-100
 *   │              └── 📄 batch_2.json // Сообщения 101-200
 *   │
 *   └── 📁 group_chats/                // Групповые чаты
 *        └── 📁 {group_id}/            // Уникальный ID группы
 *             ├── 📄 metadata.json     // Название, аватар, список участников, админы
 *             ├── 📄 pinned_messages.json // Закрепленные сообщения
 *             └── 📁 messages/
 *                  ├── 📄 batch_1.json
 *                  └── 📄 batch_2.json
 *
 *
*/

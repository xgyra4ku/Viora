#include "../inc/ChatSystem.h"

#include <filesystem>

ChatSystem::ChatSystem() {
    fs::create_directory("E:/Viora/");
    ROOT_DIR = new fs::path( "E:/Viora/");
    create_directory(*ROOT_DIR / "users");
    create_directory(*ROOT_DIR / "private_chats");
    create_directory(*ROOT_DIR / "group_chats");
}

ChatSystem::~ChatSystem() {
    delete ROOT_DIR;
}

PrivateChat ChatSystem::createPrivateChat(const unsigned int &userID1, const unsigned int &userID2) {
    PrivateChat Chat;
    Chat.setNameFolder(std::to_string(userID1) + "_" + std::to_string(userID2));
    Chat.setIDChat((userID1 + userID2) / 2);
    create_directory(*ROOT_DIR / "private_chats" / (std::to_string(userID1) + "_" + std::to_string(userID2)));
    create_directory(*ROOT_DIR / "private_chats" / (std::to_string(userID1) + "_" + std::to_string(userID2)) / "messages");
    return Chat;
}

// GroupChat ChatSystem::createGroupChat(const unsigned int &userID, const std::string &groupName) {
//     GroupChat Chat;
//     unsigned int id;
//     while ()
//     Chat.setIDChat(0);
//     Chat.setNameFolder();
//     create_directory(*ROOT_DIR / "private_chats" / "std::to_string(userID1) + _' + std::to_string(userID2)");
//     return Chat;
// }
//
// bool ChatSystem::idCheck(unsigned int id) {
//
// }



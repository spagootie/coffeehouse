#include "ChatService.h"

std::vector<std::string> ChatService::getAllUsers() {
    return repo.listUsers();
}

int ChatService::createUser(const std::string& username,
                            const std::string& displayName) {
    return repo.addUser(username, displayName);
}

bool ChatService::postMessage(int convoId, int senderId, const std::string& content) {
    if (content.empty()) return false;
    return repo.sendMessage(convoId, senderId, content);
}


bool ChatService::updateUser(int id, const std::string& username, const std::string& displayName) {
    if (id <= 0) return false;
    if (username.empty()) return false;
    return repo.updateUser(id, username, displayName);
}

bool ChatService::removeUser(int id) {
    if (id <= 0) return false;
    return repo.deleteUser(id);
}

std::vector<Message> ChatService::fetchMessages(int convoId) {
    return repo.getMessagesWithUsers(convoId); // now contains sender + text
}

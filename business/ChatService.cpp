#include "ChatService.h"

ChatService::ChatService() : repo() {}


// ---------- USERS ----------

std::vector<std::string> ChatService::getAllUsers() {
    return repo.listUsers();
}

int ChatService::createUser(const std::string& username, const std::string& displayName) {
    return repo.addUser(username, displayName);
}

bool ChatService::updateUser(int id, const std::string& username, const std::string& displayName) {
    return repo.updateUser(id, username, displayName);
}

bool ChatService::removeUser(int id) {
    return repo.deleteUser(id);
}


// ---------- MESSAGES ----------

bool ChatService::postMessage(int convoId, int senderId, const std::string& content) {
    return repo.sendMessage(convoId, senderId, content);
}

std::vector<Message> ChatService::fetchMessages(int convoId) {
    return repo.getMessagesWithUsers(convoId);
}

bool ChatService::editMessage(int messageId, const std::string& newContent) {
    return repo.updateMessage(messageId, newContent);
}

bool ChatService::removeMessage(int messageId) {
    return repo.deleteMessage(messageId);
}


// ---------- CONVERSATIONS ----------

int ChatService::createConversation(const std::string& title) {
    return repo.createConversation(title);
}

std::vector<std::pair<int,std::string>> ChatService::getAllConversations() {
    return repo.listConversations();
}

bool ChatService::removeConversation(int convoId) {
    return repo.deleteConversation(convoId);
}


// ---------- PARTICIPANTS ----------

bool ChatService::addParticipant(int convoId, int userId) {
    return repo.addParticipant(convoId, userId);
}

bool ChatService::removeParticipant(int convoId, int userId) {
    return repo.removeParticipant(convoId, userId);
}

std::vector<int> ChatService::listParticipants(int convoId) {
    return repo.getParticipants(convoId);
}

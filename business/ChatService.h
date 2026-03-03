#ifndef CHAT_SERVICE_H
#define CHAT_SERVICE_H

#include "../data/ChatRepository.h"

class ChatService {
private:
    ChatRepository repo;

public:
    std::vector<std::string> getAllUsers();
    int createUser(const std::string& username, const std::string& displayName);

    bool postMessage(int convoId, int senderId, const std::string& content);
    std::vector<Message> fetchMessages(int convoId);
    bool updateUser(int id, const std::string& username, const std::string& displayName);
    bool removeUser(int id);


};

#endif

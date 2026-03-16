#ifndef CHAT_SERVICE_H
#define CHAT_SERVICE_H

#include <string>
#include <vector>
#include "../data/ChatRepository.h"

class ChatService {
public:
    ChatService();

    // Users
    std::vector<std::string> getAllUsers();
    int createUser(const std::string& username, const std::string& displayName);
    bool updateUser(int id, const std::string& username, const std::string& displayName);
    bool removeUser(int id);

    // Messages
    bool postMessage(int convoId, int senderId, const std::string& content);
    std::vector<Message> fetchMessages(int convoId);

    // Conversations
    int createConversation(const std::string& title);
    std::vector<std::pair<int,std::string>> getAllConversations();
    bool removeConversation(int convoId);

    // Participants
    bool addParticipant(int convoId, int userId);
    bool removeParticipant(int convoId, int userId);
    std::vector<int> listParticipants(int convoId);

    // Message editing
    bool editMessage(int messageId, const std::string& newContent);
    bool removeMessage(int messageId);

private:
    ChatRepository repo;
};

#endif

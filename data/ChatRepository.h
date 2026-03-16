// data/ChatRepository.h
#ifndef CHAT_REPOSITORY_H
#define CHAT_REPOSITORY_H

#include <mysql/mysql.h>
#include <string>
#include <vector>

struct Message {
    int message_id{0};    // <- added
    std::string sender;
    std::string text;
};

class ChatRepository {
public:
    ChatRepository();
    ~ChatRepository();

    int getLastInsertId();

    std::vector<std::string> listUsers();
    int addUser(const std::string& username, const std::string& displayName);
    bool updateUser(int id, const std::string& username, const std::string& displayName);
    bool deleteUser(int id);

    bool sendMessage(int convoId, int senderId, const std::string& content);
    std::vector<std::string> getMessages(int convoId);
    std::vector<Message> getMessagesWithUsers(int convoId);

    int createConversation(const std::string& title);
    std::vector<std::pair<int, std::string>> listConversations();
    bool deleteConversation(int convoId);

    bool addParticipant(int convoId, int userId);
    bool removeParticipant(int convoId, int userId);
    std::vector<int> getParticipants(int convoId);

    bool updateMessage(int messageId, const std::string& newContent);
    bool deleteMessage(int messageId);

private:
    MYSQL *conn;
};

#endif // CHAT_REPOSITORY_H

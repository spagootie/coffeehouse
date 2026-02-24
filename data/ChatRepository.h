#ifndef CHAT_REPOSITORY_H
#define CHAT_REPOSITORY_H

#include <mysql/mysql.h>
#include <string>
#include <vector>


class ChatRepository {
private:
    MYSQL* conn;

public:
    ChatRepository();
    ~ChatRepository();

    std::vector<std::string> listUsers();
    int addUser(const std::string& username, const std::string& displayName);

    bool sendMessage(int convoId, int senderId, const std::string& content);
    std::vector<std::string> getMessages(int convoId);
    bool updateUser(int id, const std::string& username, const std::string& displayName);
    bool deleteUser(int id);

    int getLastInsertId();


};

#endif

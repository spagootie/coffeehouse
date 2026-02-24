#include "ChatRepository.h"
#include <sstream>
#include <iostream>

const char* HOST = "localhost";
const char* USER = "chat_user";
const char* PASS = "chatpass";
const char* DB   = "chat_app";

ChatRepository::ChatRepository() {
    conn = mysql_init(nullptr);
    if (!mysql_real_connect(conn, HOST, USER, PASS, DB, 0, nullptr, 0)) {
        std::cerr << "DB Connection failed\n";
        exit(1);
    }
}

ChatRepository::~ChatRepository() {
    mysql_close(conn);
}

int ChatRepository::getLastInsertId() {
    return mysql_insert_id(conn);
}

std::vector<std::string> ChatRepository::listUsers() {
    std::vector<std::string> users;

    mysql_query(conn, "SELECT username FROM users");
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
        users.push_back(row[0]);
    }

    mysql_free_result(res);
    return users;
}

int ChatRepository::addUser(const std::string& username,
                            const std::string& displayName) {
    std::stringstream query;
    query << "INSERT INTO users (username, display_name) VALUES ('"
          << username << "', '" << displayName << "')";

    if (mysql_query(conn, query.str().c_str()) == 0) {
        return mysql_insert_id(conn);
    }

    return -1;
}

bool ChatRepository::sendMessage(int convoId, int senderId, const std::string& content) {
    std::stringstream query;
    query << "INSERT INTO messages (conversation_id, sender_id, content) VALUES ("
          << convoId << ", " << senderId << ", '" << content << "')";

    return mysql_query(conn, query.str().c_str()) == 0;
}

std::vector<std::string> ChatRepository::getMessages(int convoId) {
    std::vector<std::string> messages;

    std::stringstream query;
    query << "SELECT content FROM messages WHERE conversation_id = "
          << convoId;

    mysql_query(conn, query.str().c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
        messages.push_back(row[0]);
    }

    mysql_free_result(res);
    return messages;
}

bool ChatRepository::updateUser(int id, const std::string& username, const std::string& displayName) {
    std::stringstream query;
    query << "UPDATE users SET username='"
          << username << "', display_name='"
          << displayName << "' WHERE user_id=" << id;

    return mysql_query(conn, query.str().c_str()) == 0;
}

bool ChatRepository::deleteUser(int id) {
    std::stringstream query;
    query << "DELETE FROM users WHERE user_id=" << id;

    return mysql_query(conn, query.str().c_str()) == 0;
}

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

// data/ChatRepository.cpp
std::vector<Message> ChatRepository::getMessagesWithUsers(int convoId) {
    std::vector<Message> messages;

    std::stringstream query;
    query << "SELECT m.message_id, u.username, m.content "
          << "FROM messages m "
          << "JOIN users u ON m.sender_id = u.user_id "
          << "WHERE m.conversation_id = " << convoId
          << " ORDER BY m.message_id ASC"; // keep chronological order

    if (mysql_query(conn, query.str().c_str()) != 0) {
        return messages;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
        Message msg;
        // row[0] = message_id, row[1] = username, row[2] = content
        msg.message_id = row[0] ? atoi(row[0]) : 0;
        msg.sender = row[1] ? row[1] : "";
        msg.text = row[2] ? row[2] : "";
        messages.push_back(msg);
    }

    mysql_free_result(res);
    return messages;
}
int ChatRepository::createConversation(const std::string& title) {
    std::stringstream q;
    q << "INSERT INTO conversations (title) VALUES ('" << title << "')";
    if (mysql_query(conn, q.str().c_str()) == 0) return mysql_insert_id(conn);
    return -1;
}

std::vector<std::pair<int, std::string>> ChatRepository::listConversations() {
    std::vector<std::pair<int, std::string>> out;
    mysql_query(conn, "SELECT conversation_id, IFNULL(title,'') FROM conversations");
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        int id = atoi(row[0]);
        std::string title = row[1] ? row[1] : "";
        out.emplace_back(id, title);
    }
    mysql_free_result(res);
    return out;
}

bool ChatRepository::deleteConversation(int convoId) {
    std::stringstream q;
    q << "DELETE FROM conversations WHERE conversation_id=" << convoId;
    return mysql_query(conn, q.str().c_str()) == 0;
}

bool ChatRepository::addParticipant(int convoId, int userId) {
    std::stringstream q;
    q << "INSERT IGNORE INTO participants (conversation_id, user_id) VALUES ("
      << convoId << ", " << userId << ")";
    return mysql_query(conn, q.str().c_str()) == 0;
}

bool ChatRepository::removeParticipant(int convoId, int userId) {
    std::stringstream q;
    q << "DELETE FROM participants WHERE conversation_id=" << convoId
      << " AND user_id=" << userId;
    return mysql_query(conn, q.str().c_str()) == 0;
}

std::vector<int> ChatRepository::getParticipants(int convoId) {
    std::vector<int> out;
    std::stringstream q;
    q << "SELECT user_id FROM participants WHERE conversation_id=" << convoId;
    mysql_query(conn, q.str().c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        out.push_back(atoi(row[0]));
    }
    mysql_free_result(res);
    return out;
}

bool ChatRepository::updateMessage(int messageId, const std::string& newContent) {
    std::stringstream q;
    q << "UPDATE messages SET content='" << newContent
      << "', updated_at=CURRENT_TIMESTAMP WHERE message_id=" << messageId;
    return mysql_query(conn, q.str().c_str()) == 0;
}

bool ChatRepository::deleteMessage(int messageId) {
    std::stringstream q;
    q << "DELETE FROM messages WHERE message_id=" << messageId;
    return mysql_query(conn, q.str().c_str()) == 0;
}

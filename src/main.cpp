#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

const char* HOST = "localhost";
const char* USER = "chat_user";
const char* PASS = "chatpass";
const char* DB   = "chat_app";

MYSQL* connectDB() {
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) {
        cerr << "mysql_init failed\n";
        exit(1);
    }
    if (!mysql_real_connect(conn, HOST, USER, PASS, DB, 0, nullptr, 0)) {
        cerr << "Connection failed: " << mysql_error(conn) << "\n";
        exit(1);
    }
    return conn;
}

void listUsers(MYSQL* conn) {
    if (mysql_query(conn, "SELECT user_id, username, display_name FROM users")) {
        cerr << "Query failed: " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    cout << "\nUsers:\n";
    while ((row = mysql_fetch_row(res))) {
        cout << row[0] << " | " << row[1] << " | " << row[2] << "\n";
    }
    mysql_free_result(res);
}

void listConversations(MYSQL* conn) {
    if (mysql_query(conn, "SELECT conversation_id, created_at FROM conversations")) {
        cerr << "Query failed: " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    cout << "\nConversations:\n";
    while ((row = mysql_fetch_row(res))) {
        cout << "Conversation " << row[0] << " | Created at " << row[1] << "\n";
    }
    mysql_free_result(res);
}

void showParticipants(MYSQL* conn) {
    int convoId;
    cout << "Enter conversation ID: ";
    cin >> convoId;
    stringstream query;
    query << "SELECT u.user_id, u.username "
          << "FROM conversation_participants cp "
          << "JOIN users u ON cp.user_id = u.user_id "
          << "WHERE cp.conversation_id = " << convoId;

    if (mysql_query(conn, query.str().c_str())) {
        cerr << "Query failed: " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    cout << "\nParticipants:\n";
    while ((row = mysql_fetch_row(res))) {
        cout << row[0] << " | " << row[1] << "\n";
    }
    mysql_free_result(res);
}

void showMessages(MYSQL* conn) {
    int convoId;
    cout << "Enter conversation ID: ";
    cin >> convoId;

    stringstream query;
    query << "SELECT u.username, m.content, m.sent_at "
          << "FROM messages m "
          << "JOIN users u ON m.sender_id = u.user_id "
          << "WHERE m.conversation_id = " << convoId
          << " ORDER BY m.sent_at";

    if (mysql_query(conn, query.str().c_str())) {
        cerr << "Query failed: " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    cout << "\nMessages:\n";
    while ((row = mysql_fetch_row(res))) {
        cout << "[" << row[2] << "] " << row[0] << ": " << row[1] << "\n";
    }
    mysql_free_result(res);
}

void addUser(MYSQL* conn) {
    string username, display_name;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter display name: ";
    cin.ignore();
    getline(cin, display_name);

    stringstream query;
    query << "INSERT INTO users (username, display_name) VALUES ("
          << "'" << username << "', '" << display_name << "')";

    if (mysql_query(conn, query.str().c_str())) {
        cerr << "Insert failed: " << mysql_error(conn) << "\n";
        return;
    }
    cout << "User added.\n";
}

void sendMessage(MYSQL* conn) {
    int convoId, senderId;
    string content;

    cout << "Enter conversation ID: ";
    cin >> convoId;
    cout << "Enter sender user ID: ";
    cin >> senderId;
    cin.ignore();
    cout << "Enter message content: ";
    getline(cin, content);

    stringstream query;
    query << "INSERT INTO messages (conversation_id, sender_id, content) VALUES ("
          << convoId << ", " << senderId << ", '" << content << "')";

    if (mysql_query(conn, query.str().c_str())) {
        cerr << "Insert failed: " << mysql_error(conn) << "\n";
        return;
    }
    cout << "Message sent.\n";
}

int main() {
    MYSQL* conn = connectDB();
    int choice;

    while (true) {
        cout << "\n--- Chat App ---\n";
        cout << "1. List users\n";
        cout << "2. List conversations\n";
        cout << "3. Show participants\n";
        cout << "4. Show messages\n";
        cout << "5. Add user\n";
        cout << "6. Send message\n";
        cout << "7. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: listUsers(conn); break;
            case 2: listConversations(conn); break;
            case 3: showParticipants(conn); break;
            case 4: showMessages(conn); break;
            case 5: addUser(conn); break;
            case 6: sendMessage(conn); break;
            case 7: mysql_close(conn); return 0;
            default: cout << "Invalid choice\n";
        }
    }
}


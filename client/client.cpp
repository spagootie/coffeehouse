#include "httplib.h"
#include <iostream>
#include <string>

using namespace std;

int main() {

    httplib::Client cli("localhost", 18080);

    cout << "\n===== CHAT SYSTEM TEST CLIENT =====\n";

    // -------------------------
    // Create user
    // -------------------------

    cout << "\nCreating user...\n";

    auto res = cli.Post(
        "/users",
        R"({"username":"testuser","displayName":"Test User"})",
        "application/json"
    );

    if (res && res->status == 200)
        cout << "User created: " << res->body << endl;
    else
        cout << "Failed to create user\n";



    // -------------------------
    // Get all users
    // -------------------------

    cout << "\nFetching users...\n";

    res = cli.Get("/users");

    if (res)
        cout << res->body << endl;



    // -------------------------
    // Create conversation
    // -------------------------

    cout << "\nCreating conversation...\n";

    res = cli.Post(
        "/conversations",
        R"({"title":"Test Conversation"})",
        "application/json"
    );

    if (res)
        cout << res->body << endl;



    // -------------------------
    // Add participant
    // -------------------------

    cout << "\nAdding participant...\n";

    res = cli.Post(
        "/conversations/1/participants",
        R"({"userId":1})",
        "application/json"
    );

    if (res)
        cout << "Participant added\n";



    // -------------------------
    // Send message
    // -------------------------

    cout << "\nSending message...\n";

    res = cli.Post(
        "/messages",
        R"({"convoId":1,"senderId":1,"text":"Hello from test client"})",
        "application/json"
    );

    if (res)
        cout << "Message sent\n";



    // -------------------------
    // Fetch messages
    // -------------------------

    cout << "\nFetching messages...\n";

    res = cli.Get("/messages/1");

    if (res)
        cout << res->body << endl;



    // -------------------------
    // Update message
    // -------------------------

    cout << "\nUpdating message...\n";

    res = cli.Put(
        "/messages/1",
        R"({"content":"Edited message text"})",
        "application/json"
    );

    if (res)
        cout << "Message updated\n";



    // -------------------------
    // Delete message
    // -------------------------

    cout << "\nDeleting message...\n";

    res = cli.Delete("/messages/1");

    if (res)
        cout << "Message deleted\n";



    cout << "\n===== TEST COMPLETE =====\n";

    return 0;
}

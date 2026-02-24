#include "../crow/include/crow.h"
#include "../business/ChatService.h"

/*
HOSTING INSTRUCTIONS:

Compile:
g++ server.cpp -o server -lmysqlclient -lpthread

Run:
./server

Service runs on:
http://localhost:18080

To deploy:
- Push to GitHub
- Use Render.com
- Build command: g++ server.cpp -o server -lmysqlclient -lpthread
- Start command: ./server
*/

int main() {
    crow::SimpleApp app;
    ChatService service;

    CROW_ROUTE(app, "/users")
    ([&service]() {
        auto users = service.getAllUsers();
        crow::json::wvalue result;
        result["users"] = users;
        return result;
    });

    CROW_ROUTE(app, "/users").methods("POST"_method)
([&service](const crow::request& req) {
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400);

    int newId = service.createUser(
        std::string(body["username"].s()),
        std::string(body["displayName"].s())
    );

    if (newId < 0)
        return crow::response(400);

    crow::json::wvalue result;
    result["user_id"] = newId;

    return crow::response(200, result);
});

    CROW_ROUTE(app, "/messages/<int>")
    ([&service](int convoId) {
        auto messages = service.fetchMessages(convoId);
        crow::json::wvalue result;
        result["messages"] = messages;
        return result;
    });

    CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)
([&service](const crow::request& req, int id) {
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400);

    bool success = service.updateUser(
        id,
        std::string(body["username"].s()),
        std::string(body["displayName"].s())
    );

    return crow::response(success ? 200 : 400);
});

CROW_ROUTE(app, "/users/<int>").methods("DELETE"_method)
([&service](int id) {
    bool success = service.removeUser(id);
    return crow::response(success ? 200 : 400);
});

    app.port(18080).multithreaded().run();
}

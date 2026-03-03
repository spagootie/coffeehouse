#include "../crow/include/crow.h"
#include "../business/ChatService.h"
#include <fstream>
#include <sstream>

int main() {
    crow::SimpleApp app;
    ChatService service;

    // Serve frontend
    CROW_ROUTE(app, "/")
    ([](){
        std::ifstream file("client/index.html");
        if (!file.is_open())
            return crow::response(500, "index.html not found");

        std::stringstream buffer;
        buffer << file.rdbuf();
        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html");
        return res;
    });

    // GET all users
    CROW_ROUTE(app, "/users")
    ([&service]() {
        auto users = service.getAllUsers(); // vector<string>
        crow::json::wvalue result;
        result["users"] = users;
        return result;
    });

    // POST a new user
    CROW_ROUTE(app, "/users").methods("POST"_method)
    ([&service](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("displayName"))
            return crow::response(400);

        int newId = service.createUser(body["username"].s(), body["displayName"].s());
        if (newId < 0) return crow::response(400);

        crow::json::wvalue res;
        res["user_id"] = newId;
        return crow::response(200, res);
    });

    // GET messages for a conversation (return array of {senderId, text})
    CROW_ROUTE(app, "/messages/<int>")
([&service](int convoId){
    auto msgs = service.fetchMessages(convoId); // vector<Message>

    crow::json::wvalue res;
    res["messages"] = crow::json::wvalue::list();

    for (size_t i = 0; i < msgs.size(); i++) {
        crow::json::wvalue obj;
        obj["sender"] = msgs[i].sender;  // real username from DB
        obj["text"] = msgs[i].text;
        res["messages"][i] = std::move(obj);
    }

    return res;
});

    // POST new message
    CROW_ROUTE(app, "/messages").methods("POST"_method)
    ([&service](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body || !body.has("convoId") || !body.has("senderId") || !body.has("text"))
            return crow::response(400);

        int convoId = body["convoId"].i();
        int senderId = body["senderId"].i();
        std::string text = body["text"].s();

        bool success = service.postMessage(convoId, senderId, text);
        return crow::response(success ? 200 : 500);
    });

    // Optional: PUT/DELETE users
    CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)
    ([&service](const crow::request& req, int id){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        bool success = service.updateUser(id, body["username"].s(), body["displayName"].s());
        return crow::response(success ? 200 : 400);
    });

    CROW_ROUTE(app, "/users/<int>").methods("DELETE"_method)
    ([&service](int id){
        bool success = service.removeUser(id);
        return crow::response(success ? 200 : 400);
    });

    app.port(18080).multithreaded().run();
}

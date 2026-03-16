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
            return crow::response(500,"index.html not found");

        std::stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header("Content-Type","text/html");

        return res;
    });


    // -------- USERS --------

    CROW_ROUTE(app,"/users")
    ([&service](){

        auto users = service.getAllUsers();

        crow::json::wvalue result;
        result["users"] = users;

        return result;
    });


    CROW_ROUTE(app,"/users").methods("POST"_method)
    ([&service](const crow::request& req){

        auto body = crow::json::load(req.body);

        if(!body || !body.has("username") || !body.has("displayName"))
            return crow::response(400);

        int id = service.createUser(
            std::string(body["username"].s()),
            std::string(body["displayName"].s())
        );

        crow::json::wvalue res;
        res["user_id"] = id;

        return crow::response(200,res);
    });


    CROW_ROUTE(app,"/users/<int>").methods("PUT"_method)
    ([&service](const crow::request& req,int id){

        auto body = crow::json::load(req.body);

        if(!body)
            return crow::response(400);

        bool ok = service.updateUser(
            id,
            std::string(body["username"].s()),
            std::string(body["displayName"].s())
        );

        return crow::response(ok?200:400);
    });


    CROW_ROUTE(app,"/users/<int>").methods("DELETE"_method)
    ([&service](int id){

        bool ok = service.removeUser(id);
        return crow::response(ok?200:400);
    });


    // -------- MESSAGES --------

CROW_ROUTE(app,"/messages/<int>")
([&service](int convoId){
    auto msgs = service.fetchMessages(convoId); // vector<Message>

    crow::json::wvalue res;
    res["messages"] = crow::json::wvalue::list();

    for (size_t i = 0; i < msgs.size(); i++) {
        crow::json::wvalue obj;
        obj["message_id"] = msgs[i].message_id;
        obj["sender"] = msgs[i].sender;
        obj["text"] = msgs[i].text;
        res["messages"][i] = std::move(obj);
    }

    return res;
});

    CROW_ROUTE(app,"/messages").methods("POST"_method)
    ([&service](const crow::request& req){

        auto body = crow::json::load(req.body);

        if(!body)
            return crow::response(400);

        bool ok = service.postMessage(
            body["convoId"].i(),
            body["senderId"].i(),
            std::string(body["text"].s())
        );

        return crow::response(ok?200:500);
    });


    CROW_ROUTE(app,"/messages/<int>").methods("PUT"_method)
    ([&service](const crow::request& req,int messageId){

        auto body = crow::json::load(req.body);

        if(!body)
            return crow::response(400);

        bool ok = service.editMessage(
            messageId,
            std::string(body["content"].s())
        );

        return crow::response(ok?200:400);
    });


    CROW_ROUTE(app,"/messages/<int>").methods("DELETE"_method)
    ([&service](int messageId){

        bool ok = service.removeMessage(messageId);
        return crow::response(ok?200:400);
    });


    // -------- CONVERSATIONS --------

    CROW_ROUTE(app,"/conversations").methods("POST"_method)
    ([&service](const crow::request& req){

        auto body = crow::json::load(req.body);

        std::string title = "Untitled";

        if(body && body.has("title"))
            title = std::string(body["title"].s());

        int id = service.createConversation(title);

        crow::json::wvalue res;
        res["conversation_id"] = id;

        return crow::response(200,res);
    });


    CROW_ROUTE(app,"/conversations")
    ([&service](){

        auto convos = service.getAllConversations();

        crow::json::wvalue res;
        res["conversations"] = crow::json::wvalue::list();

        for(size_t i=0;i<convos.size();i++){

            crow::json::wvalue c;

            c["conversation_id"] = convos[i].first;
            c["title"] = convos[i].second;

            res["conversations"][i] = std::move(c);
        }

        return res;
    });


    // -------- PARTICIPANTS --------

    CROW_ROUTE(app,"/conversations/<int>/participants").methods("POST"_method)
    ([&service](const crow::request& req,int convoId){

        auto body = crow::json::load(req.body);

        if(!body || !body.has("userId"))
            return crow::response(400);

        bool ok = service.addParticipant(convoId,body["userId"].i());

        return crow::response(ok?200:400);
    });


    CROW_ROUTE(app,"/conversations/<int>/participants/<int>").methods("DELETE"_method)
    ([&service](int convoId,int userId){

        bool ok = service.removeParticipant(convoId,userId);
        return crow::response(ok?200:400);
    });


    CROW_ROUTE(app,"/conversations/<int>/participants")
    ([&service](int convoId){

        auto parts = service.listParticipants(convoId);

        crow::json::wvalue res;
        res["participants"] = crow::json::wvalue::list();

        for(size_t i=0;i<parts.size();i++)
            res["participants"][i] = parts[i];

        return res;
    });


    app.port(18080).multithreaded().run();
}

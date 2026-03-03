#include "../crow/include/crow.h"
#include "httplib.h"

int main() {
    crow::SimpleApp app;

    // =========================
    // HTML UI
    // =========================
    CROW_ROUTE(app, "/")([]() {
        crow::response res;
        res.set_header("Content-Type", "text/html");

        res.body = R"HTML(
<!DOCTYPE html>
<html>
<head>
<title>Chat Client</title>
<style>
body { margin:0; font-family:Arial; display:flex; height:100vh; }
#sidebar { width:250px; background:#2f3136; color:white; padding:10px; overflow-y:auto; }
.user { padding:8px; cursor:pointer; border-radius:4px; }
.user:hover { background:#40444b; }
#chat { flex:1; display:flex; flex-direction:column; background:#36393f; }
#chatHeader { padding:10px; background:#202225; color:white; }
#messages { flex:1; padding:10px; overflow-y:auto; color:white; }
.message { margin-bottom:10px; padding:8px; background:#40444b; border-radius:6px; }
</style>
</head>
<body onload="loadUsers()">

<div id="sidebar">
<h2>Users</h2>
<div id="userList"></div>
</div>

<div id="chat">
<div id="chatHeader">Select a user</div>
<div id="messages"></div>
</div>

<script>

let currentUserId = null;

// -------------------
// Load All Users
// -------------------
async function loadUsers() {
    const res = await fetch('/api/users');
    const data = await res.json();

    const list = document.getElementById('userList');
    list.innerHTML = '';

    if (!data.users) {
        list.innerHTML = "No users found.";
        return;
    }

    data.users.forEach(u => {

        // Dynamically determine ID field
        const id = u.id || u.user_id;
        const name = u.displayName || u.display_name || u.username;

        const div = document.createElement('div');
        div.className = 'user';
        div.innerText = name + " (" + id + ")";
        div.onclick = () => selectUser(id);

        list.appendChild(div);
    });
}

// -------------------
// Load Single User
// -------------------
async function selectUser(id) {
    currentUserId = id;

    const res = await fetch('/api/users/' + id);
    const data = await res.json();

    const user = data.user || data;

    const name = user.displayName || user.display_name || user.username;

    document.getElementById('chatHeader').innerText =
        "Chat with: " + name;

    loadMessages(id);
}

// -------------------
// Load Messages
// -------------------
async function loadMessages(id) {
    const res = await fetch('/api/messages/' + id);
    const data = await res.json();

    const box = document.getElementById('messages');
    box.innerHTML = '';

    if (!data.messages) return;

    data.messages.forEach(m => {

        const sender = m.sender_id || m.sender || "User";
        const content = m.content || m.message || "";

        const div = document.createElement('div');
        div.className = 'message';
        div.innerText = sender + ": " + content;

        box.appendChild(div);
    });

    box.scrollTop = box.scrollHeight;
}

</script>
</body>
</html>
)HTML";

        return res;
    });

    // =========================
    // API PROXY ROUTES
    // =========================

    CROW_ROUTE(app, "/api/users")([]() {
        httplib::Client cli("localhost", 18080);
        auto res = cli.Get("/users");
        return crow::response(res ? res->body : "{}");
    });

    CROW_ROUTE(app, "/api/users/<int>")
    ([](int id) {
        httplib::Client cli("localhost", 18080);
        auto res = cli.Get(("/users/" + std::to_string(id)).c_str());
        return crow::response(res ? res->body : "{}");
    });

    CROW_ROUTE(app, "/api/messages/<int>")
    ([](int id) {
        httplib::Client cli("localhost", 18080);
        auto res = cli.Get(("/messages/" + std::to_string(id)).c_str());
        return crow::response(res ? res->body : "{}");
    });

    app.port(8080).multithreaded().run();
}

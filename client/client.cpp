/*#include <iostream>
#include "httplib.h"

int main() {
    httplib::Client cli("localhost", 18080);

    auto res = cli.Get("/users");
    if (res) {
        std::cout << res->body << std::endl;
    }

    return 0;
}*/

#include <iostream>
#include <string>
#include "httplib.h"
#include "../crow/include/crow.h"

void printResponse(const std::string& label, const httplib::Result& res) {
    std::cout << "\n===== " << label << " =====\n";
    if (res) {
        std::cout << "Status: " << res->status << "\n";
        std::cout << "Body:\n" << res->body << "\n";
    } else {
        std::cout << "Request failed.\n";
    }
}
int main() {
    httplib::Client cli("localhost", 18080);

    // STEP 1: INSERT NEW USER
    std::string newUserJson =
        R"({"username":"crud_test_user","displayName":"CRUD Test User"})";

    auto insertRes = cli.Post("/users", newUserJson, "application/json");
    printResponse("INSERT USER", insertRes);

    if (!insertRes || insertRes->status != 200) {
        std::cout << "Insert failed. Exiting.\n";
        return 1;
    }

    // Parse returned JSON to get user_id
    auto insertBody = insertRes->body;
auto parsed = crow::json::load(insertRes->body);

    if (!parsed || !parsed.has("user_id")) {
        std::cout << "Failed to parse inserted user ID.\n";
        return 1;
    }

    int testUserId = parsed["user_id"].i();

    std::cout << "Inserted user ID: " << testUserId << "\n";

    // STEP 2: GET USERS
    auto getAfterInsert = cli.Get("/users");
    printResponse("GET AFTER INSERT", getAfterInsert);

    // STEP 3: UPDATE USER
    std::string updateJson =
        R"({"username":"updated_crud_user","displayName":"Updated CRUD User"})";

    auto updateRes = cli.Put(("/users/" + std::to_string(testUserId)).c_str(),
                             updateJson,
                             "application/json");

    printResponse("UPDATE USER", updateRes);

    // STEP 4: GET USERS AGAIN
    auto getAfterUpdate = cli.Get("/users");
    printResponse("GET AFTER UPDATE", getAfterUpdate);

    // STEP 5: DELETE USER
    auto deleteRes = cli.Delete(("/users/" + std::to_string(testUserId)).c_str());
    printResponse("DELETE USER", deleteRes);

    // STEP 6: FINAL GET
    auto getAfterDelete = cli.Get("/users");
    printResponse("GET AFTER DELETE", getAfterDelete);

    return 0;
}

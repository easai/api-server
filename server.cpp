#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::unordered_map<std::string, std::string> db;
const std::string db_file = "data.json";

#define NOT_FOUND "Not found in the dictionary"

void load_db() {
    std::ifstream file(db_file);
    if (file) {
        json j;
        file >> j;
        for (auto& [k, v] : j.items())
            db[k] = v;
    }
}

void save_db() {
    json j(db);
    std::ofstream file(db_file);
    file << j.dump(4);
}

std::string handle_logic(const std::string& req) {
    std::istringstream stream(req);
    std::string method, key, value;
    stream >> method >> key >> value;

    json j;
    j["method"] = method;

    if (method == "CREATE") {
        db[key] = value;
        save_db();
        j["status"] = "success";
        j["message"] = "Created " + key;
        j["key"] = key;
        j["value"] = value;
    } else if (method == "READ") {
        if (db.count(key)) {
            j["status"] = "success";
            j["key"] = key;
            j["value"] = db[key];
        } else {
            j["status"] = "error";
            j["message"] = "Key not found";
        }
    } else if (method == "UPDATE") {
        if (db.count(key)) {
            db[key] = value;
            save_db();
            j["status"] = "success";
            j["message"] = "Updated " + key;
            j["key"] = key;
            j["value"] = value;
        } else {
            j["status"] = "error";
            j["message"] = "Key not found";
        }
    } else if (method == "DELETE") {
        if (db.erase(key)) {
            save_db();
            j["status"] = "success";
            j["message"] = "Deleted " + key;
            j["key"] = key;
        } else {
            j["status"] = "error";
            j["message"] = "Key not found";
        }
    } else if (method == "DUMP") {
        j = db; // return all entries as JSON
    } else {
        j["status"] = "error";
        j["message"] = "Invalid method";
    }

    return j.dump();
}


std::string make_http_response(const std::string& body) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Content-Type: application/json\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "\r\n";
    response << body;
    return response.str();
}

int main() {
    load_db();
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    std::cout << "HTTP+CRUD server running on port 8080...\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        char buffer[2048] = {};
        int bytes_read = read(client_fd, buffer, sizeof(buffer));

        std::string full_req(buffer, bytes_read);
        auto body_pos = full_req.find("\r\n\r\n");
        std::string body = body_pos != std::string::npos ? full_req.substr(body_pos + 4) : "";

        std::string logic_response = handle_logic(body);
        std::string http_response = make_http_response(logic_response);

        send(client_fd, http_response.c_str(), http_response.size(), 0);
        close(client_fd);
    }

    close(server_fd);
}

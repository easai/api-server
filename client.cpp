#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

std::string send_request(const std::string& msg) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) return "Socket creation failed";

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        return "Connection failed";

    send(sock, msg.c_str(), msg.size(), 0);

    char buffer[1024] = {};
    read(sock, buffer, sizeof(buffer));
    close(sock);

    return std::string(buffer);
}

int main() {
    std::cout << "Tungjatjeta:\n";
    std::cout << "   CREATE key value\n   READ key\n   UPDATE key value\n   DELETE key\n";
    std::cout << "Type EXIT to quit.\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "EXIT") break;

        std::string response = send_request(line);
        std::cout << "🟢 Server: " << response << "\n";
    }

    std::cout << "👋 Client exited.\n";
    return 0;
}

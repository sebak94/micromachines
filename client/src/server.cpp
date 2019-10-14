#include "../include/server.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include "string.h"
#include <vector>
#include <string>

Server::Server(const char *hostname, const char *service) {
    try {
        skt.Connect(hostname, service);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

void Server::send(std::string command) {
    const char *cmd = command.c_str();
    skt.Send(cmd, command.size());
}

std::string Server::receive() {
    std::vector<char> response;
    char c;  
    skt.Receive(&c, 1);
    while (c != '\n') {
        response.push_back(c);
        skt.Receive(&c, 1);
    }
    std::string str_resp(response.begin(), response.end());
    return str_resp + "\n";
}

Server::~Server() {}

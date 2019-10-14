#include <iostream>
#include <string>
#include "include/server.h"
#include "../common/include/socket_error.h"

#define SUCCESS 0

void run(const char *hostname, const char *service) {
    try {
        bool running = true;
        std::string str;
        Server server(hostname, service);

        while (running) {
            std::cout << server.receive();
            running = false;
        }
    } catch(const SocketError &e) {}
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cout << "./client <hostname> <service>";
        return SUCCESS;
    }
    run(argv[1], argv[2]);
    return SUCCESS;
}

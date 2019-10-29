#include "../include/server.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include <vector>
#include <string>

#define LIMIT_QUEUE 1000 //elijo un numero maximo cualquiera

Server::Server(const char *hostname, const char *service) : queue(LIMIT_QUEUE) {
    try {
        skt.Connect(hostname, service);
        printf("socket conectado\n");
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

Server::~Server() {
    for (int i = 0; i < threads.size(); i++) {
        delete threads[i];
    }
}

void Server::runThreads() {
    threads.push_back(new EventLoopSDL(queue)); //o evento de Lua
    threads.push_back(new Dispatcher(queue, skt));
    threads.push_back(new Drawer(skt));

    for (int i = 0; i < threads.size(); i++) {
        threads[i]->start();
    }
    for (int i = 0; i < threads.size(); i++) {
        threads[i]->join();
        printf("joineado el thread: %d\n", i);
    }
}

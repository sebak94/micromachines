#include "../include/mm_server.h"
#include "../include/acceptor_th.h"
#include "../include/game_loop_th.h"
#include "../include/model/micromachines_th.h"
#include "../../common/include/thread.h"
#include "../../common/include/socket_error.h"
#include "../include/games_th.h"
#include <iostream>
#include <string>
#include <vector>

void MMServer::startThreads() {
    for (std::vector<Thread*>::iterator it = threads.begin();
        it != threads.end(); ++it) {
        (*it)->start();
    }
}

void MMServer::finishThreads() {
    for (std::vector<Thread*>::iterator it = threads.begin();
        it != threads.end(); ++it) {
        (*it)->stop();
        (*it)->join();
    }
}

void MMServer::start(const char *service) {
    try {
        GamesTh games;
        AcceptorTh acceptor_th(service, games);
        threads.push_back(&acceptor_th);
        threads.push_back(&games);
        startThreads();
        while (getc(stdin) != 'q') {}
        finishThreads();
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

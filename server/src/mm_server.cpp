#include "../include/mm_server.h"
#include "../include/acceptor_th.h"
#include "../include/game_loop_th.h"
#include "../include/model/micromachines.h"
#include "../../common/include/thread.h"
#include "../../common/include/socket_error.h"
#include <iostream>
#include <string>
#include <vector>

void MMServer::startThreads() {
    for (std::vector<Thread*>::iterator it = threads.begin();
        it != threads.end(); it++) {
        (*it)->start();
    }
}

void MMServer::finishThreads() {
    for (std::vector<Thread*>::iterator it = threads.begin();
        it != threads.end(); it++) {
        (*it)->stop();
        (*it)->join();
    }
}

void MMServer::start(const char *service) {
    try {
        Micromachines micromachines;
        GameLoopTh game_loop_th(micromachines);
        AcceptorTh acceptor_th(service, micromachines);
        threads.push_back(&game_loop_th);
        threads.push_back(&acceptor_th);
        startThreads();
        while (getc(stdin) != 'q') {}
        finishThreads();
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

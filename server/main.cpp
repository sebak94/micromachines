#include "include/acceptor_th.h"
#include "include/game_loop_th.h"
#include "include/micromachines.h"
#include "../common/include/socket_error.h"
#include <iostream>
#include <string>
#include <vector>

#define SUCCESS 0

void startThreads(std::vector<Thread*> &threads) {
    for (std::vector<Thread*>::iterator it = threads.begin();
        it != threads.end(); it++) {
        (*it)->start();
    }
}

void finishThreads(std::vector<Thread*> &threads) {
    for (std::vector<Thread*>::iterator it = threads.begin();
        it != threads.end(); it++) {
        (*it)->stop();
        (*it)->join();
    }
}

void run(const char* service) {
    try {
        Micromachines micromachines;
        std::vector<Thread*> threads;
        GameLoopTh game_loop_th(micromachines);
        AcceptorTh acceptor_th(service, micromachines);
        threads.push_back(&game_loop_th);
        threads.push_back(&acceptor_th);
        startThreads(threads);
        while (getc(stdin) != 'q') {}
        finishThreads(threads);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cout << "El comando es: ./micromachines-sv <service>\n";
        return SUCCESS;
    }
    run(argv[1]);
    return SUCCESS;
}

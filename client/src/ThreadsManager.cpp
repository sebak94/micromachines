#include "../include/ThreadsManager.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include "../include/ModelUpdater.h"
#include <vector>
#include <string>

#define LIMIT_QUEUE 1000 //elijo un numero maximo cualquiera

ThreadsManager::ThreadsManager(const char *hostname, const char *service) : queue(LIMIT_QUEUE) {
    try {
        skt.Connect(hostname, service);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

ThreadsManager::~ThreadsManager() {
    for (auto & thread : threads) {
        delete thread;
    }
}

void ThreadsManager::runThreads() {
    Model model;
    ModelMonitor modelMonitor(model);
    Drawer* drawer = new Drawer(modelMonitor);

    threads.push_back(new EventLoopSDL(queue, drawer, modelMonitor));
    threads.push_back(new Dispatcher(queue, skt));
    threads.push_back(new ModelUpdater(skt, modelMonitor, drawer));
    threads.push_back(drawer);
    //Cuando el ModelUpdater finaliza, le hace stop al Drawer

    for (auto & thread : threads) {
        thread->start();
    }
    for (auto & thread : threads) {
        thread->join();
    }
}

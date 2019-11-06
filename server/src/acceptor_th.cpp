#include "../include/acceptor_th.h"
#include "../include/client_th.h"
#include "../include/model/micromachines.h"
#include "../../common/include/socket.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include "vector"

AcceptorTh::AcceptorTh(const char *service, Micromachines &micromachines):
    keep_accepting(true), micromachines(micromachines) {
    try {
        skt.BindAndListen(service);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

void AcceptorTh::deleteDeadClients() {
    std::vector<ClientTh*>::iterator it = clients.begin();
    while (it != clients.end()) {
        if ((*it)->isDead()) {
            micromachines.removePlayer((*it));
            (*it)->join();
            delete (*it);
            clients.erase(it);
        } else {
            it++;
        }
    }
}

void AcceptorTh::deleteResources() {
    for (size_t i = 0; i < clients.size(); i++) {
        clients[i]->stop();
        clients[i]->join();
        delete clients[i];
    }

    for (size_t i = 0; i < sockets.size(); i++) {
        delete sockets[i];
    }
}

void AcceptorTh::run() {
    while (keep_accepting) {
        try {
            Socket *peer = new Socket();
            sockets.push_back(peer);
            skt.Accept(peer);
            ClientTh *client_th = new ClientTh(peer);
            micromachines.addPlayer(client_th);
            clients.push_back(client_th);
            client_th->sendTrackData(micromachines.trackSerialized());
            client_th->start();
            deleteDeadClients();
        } catch(const SocketError &e) {
            std::cout << e.what() << "\n";
            keep_accepting = false;
        }
    }

    deleteResources();    
}

void AcceptorTh::stop() {
    keep_accepting = false;
    skt.Release();
    micromachines.cleanPlayers();
}

AcceptorTh::~AcceptorTh() {}

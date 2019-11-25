#include <unistd.h>
#include "../include/acceptor_th.h"
#include "../include/model/micromachines_th.h"
#include "../../common/include/socket_error.h"
#include "../../common/include/lock.h"
#include "iostream"
#include "vector"

AcceptorTh::AcceptorTh(const char *service, GamesTh &games):
        keep_accepting(true), games(games), clients(games) {
    try {
        looking_th = new LookForDeadClientsTh(clients, games);
        skt.BindAndListen(service);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

void AcceptorTh::deleteResources() {
   for (size_t i = 0; i < sockets.size(); i++) {
        delete sockets[i];
    }
}

void AcceptorTh::run() {
    looking_th->start();
    TrackList trackList;
    while (keep_accepting) {
        try {
            Socket *peer = new Socket();
            sockets.push_back(peer);
            skt.Accept(peer);

            ClientTh *client_th = new ClientTh(peer, trackList);
            clients.addClient(client_th);
            games.setPlayerToAssign(client_th);
            client_th->start();
        } catch(const SocketError &e) {
            std::cout << e.what() << "\n";
            keep_accepting = false;
        }
    }
    deleteResources();    
}

void AcceptorTh::stop() {
    keep_accepting = false;
    looking_th->stop();
    looking_th->join();
    skt.Release();
}

AcceptorTh::~AcceptorTh() {
    delete looking_th;
}

LookForDeadClientsTh::LookForDeadClientsTh(ClientList &clients, GamesTh &games)
                                        : keep_looking(true), clients(clients),
                                        games(games) {}

void LookForDeadClientsTh::run() {
    while (keep_looking) {
        clients.deleteDeadClients();
        usleep(DEADCLIENTSTIMEPERIOD);
    }
}

void LookForDeadClientsTh::stop() {
    keep_looking = false;
}

ClientList::ClientList(GamesTh &games):
    games(games) {}

void ClientList::addClient(ClientTh* client) {
    Lock l(m);
    clients.push_back(client);
}

void ClientList::deleteDeadClients() {
    Lock l(m);
    auto it = clients.begin();
    while (it != clients.end()) {
        if ((*it)->isDead()) {
            games.removePlayer(*it);
            (*it)->join();
            delete *it;
            it = clients.erase(it);
        } else {
            it++;
        }
    }
}

ClientList::~ClientList() {
    Lock l(m);

    for (size_t i = 0; i < clients.size(); i++) {
        clients[i]->stop();
        clients[i]->join();
        delete clients[i];
    }
}

#include "../include/acceptor_th.h"
#include "../include/client_th.h"
#include "../../common/include/socket.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include "vector"

AcceptorTh::AcceptorTh(const char *service): keep_accepting(true) {
    try {
        skt.BindAndListen(service);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }
}

void AcceptorTh::run() {
    std::vector<ClientTh*> clients;
    std::vector<Socket*> sockets;

    while (keep_accepting) {
        try {
            Socket *peer = new Socket();
            sockets.push_back(peer);
            skt.Accept(peer);
            ClientTh *client_th = new ClientTh(peer);
            clients.push_back(client_th);
            client_th->start();
            
            std::vector<ClientTh*>::iterator it = clients.begin();
            while (it != clients.end()) {
                if ((*it)->isDead()) {
                    (*it)->join();
                    delete (*it);
                    clients.erase(it);
                } else {
                    it++;
                }
            }
        } catch(const SocketError &e) {
            std::cout << e.what() << "\n";
            keep_accepting = false;
        }
    }

    for (size_t i = 0; i < clients.size(); i++) {
        clients[i]->stop();
        clients[i]->join();
        delete clients[i];
    }

    for (size_t i = 0; i < sockets.size(); i++) {
        delete sockets[i];
    }
}

void AcceptorTh::stop() {
    keep_accepting = false;
    skt.Release();
}

AcceptorTh::~AcceptorTh() {}

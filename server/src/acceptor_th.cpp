#include "../include/acceptor_th.h"
#include "../include/model/micromachines.h"
#include "../../common/include/socket_error.h"
#include "iostream"
#include "vector"
#include "../include/model/cars/blue_car.h"
#include "../include/model/cars/red_car.h"
#include "../include/model/cars/yellow_car.h"
#include "../include/model/cars/black_car.h"
#include "../include/model/cars/white_car.h"

AcceptorTh::AcceptorTh(const char *service, Micromachines &micromachines):
    keep_accepting(true), micromachines(micromachines) {
    try {
        skt.BindAndListen(service);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }

    //Agrego todos los autos disponibles en un mapa de autos
    cars[blue] = new BlueCar(micromachines.world,
        micromachines.getStartingPoint(0), micromachines.getStartingCarRot(0));
    cars[white] = new WhiteCar(micromachines.world,
        micromachines.getStartingPoint(1), micromachines.getStartingCarRot(1));
    cars[black] = new BlackCar(micromachines.world,
        micromachines.getStartingPoint(2), micromachines.getStartingCarRot(2));
    cars[yellow] = new YellowCar(micromachines.world,
        micromachines.getStartingPoint(3), micromachines.getStartingCarRot(3));
    cars[red] = new RedCar(micromachines.world,
        micromachines.getStartingPoint(4), micromachines.getStartingCarRot(4));
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
    auto it = cars.begin();
    while (keep_accepting) {
        try {
            Socket *peer = new Socket();
            sockets.push_back(peer);
            skt.Accept(peer);
            ClientTh *client_th = new ClientTh(peer, (it++)->second, micromachines.getTracks());
            micromachines.addPlayer(client_th);
            clients.push_back(client_th);
            client_th->sendAllTrackNames(micromachines.allTrackNames());
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

AcceptorTh::~AcceptorTh() {
    for (const auto pair : cars) {
        delete pair.second;
    }
}

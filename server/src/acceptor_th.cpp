#include <unistd.h>
#include "../include/acceptor_th.h"
#include "../include/model/micromachines_th.h"
#include "../../common/include/socket_error.h"
#include "../../common/include/lock.h"
#include "iostream"
#include "vector"
#include "../include/model/cars/blue_car.h"
#include "../include/model/cars/red_car.h"
#include "../include/model/cars/yellow_car.h"
#include "../include/model/cars/black_car.h"
#include "../include/model/cars/white_car.h"

AcceptorTh::AcceptorTh(const char *service, GamesTh &games):
        keep_accepting(true), games(games), clients(games) {
    try {
        looking_th = new LookForDeadClientsTh(clients, games);
        skt.BindAndListen(service);
    } catch(const SocketError &e) {
        std::cout << e.what() << "\n";
    }

    //Agrego todos los autos disponibles en un mapa de autos
    /*cars[blue] = new BlueCar(games.world,
                             games.getStartingPoint(0),
                             games.getStartingCarRot(0),
                             games.getStartID(0));
    cars[white] = new WhiteCar(games.world,
                               games.getStartingPoint(1),
                               games.getStartingCarRot(1),
                               games.getStartID(1));
    cars[black] = new BlackCar(games.world,
                               games.getStartingPoint(2),
                               games.getStartingCarRot(2),
                               games.getStartID(2));
    cars[yellow] = new YellowCar(games.world,
                                 games.getStartingPoint(3),
                                 games.getStartingCarRot(3),
                                 games.getStartID(3));
    cars[red] = new RedCar(games.world,
                           games.getStartingPoint(4),
                           games.getStartingCarRot(4),
                           games.getStartID(4));*/
}

void AcceptorTh::deleteResources() {
   for (size_t i = 0; i < sockets.size(); i++) {
        delete sockets[i];
    }
}

void AcceptorTh::run() {
    //auto it = cars.begin();
    looking_th->start();
    TrackList trackList;
    int gameNumber;
    while (keep_accepting) {
        try {
            Socket *peer = new Socket();
            sockets.push_back(peer);
            skt.Accept(peer);

            ClientTh *client_th = new ClientTh(peer, trackList);
            clients.addClient(client_th);
            games.setPlayerOnMainMenu(client_th);

            //si creo partida
            /*gameNumber = games.createGame(client_th);
            client_th->sendAllTrackNames(games.allTrackNames(gameNumber));
            client_th->sendLapsData(games.lapsSerialized(gameNumber));*/
            client_th->start();

            // si me uno
            /*games.addPlayer(client_th,0);
            client_th->sendTrackData(games.trackSerialized(0));
            client_th->sendLapsData(games.lapsSerialized(0));
            client_th->start();*/



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
    for (int i=0; i<games.getGamesNumber(); i++)
        games.cleanPlayers(i);
}

AcceptorTh::~AcceptorTh() {
    delete looking_th;
    /*for (const auto pair : cars) {
        delete pair.second;
    }*/
}

LookForDeadClientsTh::LookForDeadClientsTh(ClientList &clients,
                                          GamesTh &games): keep_looking(true), clients(clients),
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
            games.removePlayer(*it, games.getPlayerGameID(*it));
            (*it)->join();
            delete *it;
            clients.erase(it);
        } else {
            it++;
        }
    }
}

ClientList::~ClientList() {
    for (size_t i = 0; i < clients.size(); i++) {
        clients[i]->stop();
        clients[i]->join();
        delete clients[i];
    }
}

//
// Created by fperezboco on 17/11/19.
//

#ifndef MICROMACHINES_GAMES_TH_H
#define MICROMACHINES_GAMES_TH_H


#include "../../common/include/thread.h"
#include "model/micromachines_th.h"
#include "game_loop_th.h"
#include <vector>
#include <mutex>

class GamesTh : public Thread {
private:
    std::map<int, MicroMachinesTh*> games;
    std::map<int, GameLoopTh*> gameLoops;
    std::map<ClientTh*, int> players;  // saves game where is playing
    std::map<int, std::thread*> mapperThreads;
    std::map<int, bool> mapperThreadList;  // true if thread finished
    bool running = true;
    int gamesNumber = 0;
    Config config;
    std::mutex m;

public:
    GamesTh() = default;
    void run() override;
    void stop() override;
    void addPlayer(ClientTh *player);
    void removePlayer(ClientTh *player);
    void cleanPlayers();
    void createGame(ClientTh *player);
    void setPlayerToAssign(ClientTh *player);
    void processPlayer(ClientTh *player, bool &finished);
    void deleteMapperThreads();
    void mapNewClients();
    std::string serializeGames();
    void gameEndedPlayersToMainMenu();
    void joinEndedGames();
    void stopGameIfAllEnded();
    void stopAllThreads();
};


#endif //MICROMACHINES_GAMES_TH_H

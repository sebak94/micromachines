//
// Created by fperezboco on 17/11/19.
//

#ifndef MICROMACHINES_GAMES_TH_H
#define MICROMACHINES_GAMES_TH_H


#include <vector>
#include "../../common/include/thread.h"
#include "model/micromachines_th.h"
#include "game_loop_th.h"

class GamesTh : public Thread {
private:
    std::map<int, MicroMachinesTh*> games;
    std::map<int, GameLoopTh*> gameLoops;
    std::map<ClientTh*, int> players;  // saves game where is playing
    bool running = true;
    int gamesNumber = 0;

public:
    void run() override;
    void stop() override;
    void addPlayer(ClientTh *player, int gameIndex);
    void removePlayer(ClientTh *player, int gameIndex);
    void cleanPlayers(int gameIndex);
    int createGame(ClientTh *player);
    int getGamesNumber();
    std::string trackSerialized(int gameIndex);
    std::string lapsSerialized(int gameIndex);
    std::string allTrackNames(int gameIndex);
    int getPlayerGameID(ClientTh* player);
    void setPlayerOnMainMenu(ClientTh *player);

    std::string serializeGames();
};


#endif //MICROMACHINES_GAMES_TH_H

//
// Created by fperezboco on 17/11/19.
//

#include "../include/games_th.h"
#include "../include/model/micromachines_th.h"
#include "../include/game_loop_th.h"
#include <unistd.h>

#define PLAYERTOASSIGN -1
#define PLAYERBEINGASSIGNED -2
#define REFRESHPLAYERSTIME 500000  // us

void GamesTh::run() {
    while (running) {
        mapNewClients();
        deleteMapperThreads();
        usleep(REFRESHPLAYERSTIME);
    }
}

// process new player create/join in new thread
void GamesTh::mapNewClients() {
    int i = 0;
    for (auto & newPlayer : players) {
        if (newPlayer.second == PLAYERTOASSIGN) {
            newPlayer.second = PLAYERBEINGASSIGNED;
            std::thread * th;
            mapperThreadList.emplace(i, false);
            th = new std::thread(&GamesTh::processPlayer,
                                 this,
                                 newPlayer.first,
                                 std::ref(mapperThreadList.at(i)));
            mapperThreads.emplace(i, th);
            i++;
        }
    }
}

// joins mapper thread if finished
void GamesTh::deleteMapperThreads() {
    auto it2 = mapperThreads.begin();
    for (auto it1 = mapperThreadList.begin(); it1 != mapperThreadList.end();) {
        if (it1->second){
            (it2->second)->join();
            free(it2->second);
            mapperThreads.erase(it2++);
            mapperThreadList.erase(it1++);
        } else {
            ++it1, ++it2;
        }
    }
}

void GamesTh::stop() {
    running = false;
}

// decides if creates or joins
void GamesTh::processPlayer(ClientTh * player, bool & finished) {
    while (player->getState() == mainMenu) {
        player->setPlayerMode(); //setea join o create
        usleep(REFRESHPLAYERSTIME);
    }
    if (player->getState() == creating) {
        createGame(player);
        finished = true;
    } else if (player->getState() == joining) {
        addPlayer(player);
        finished = true;
    }
}

// Creates new game and adds player to it
void GamesTh::createGame(ClientTh * player) {
    TrackList tracks;
    tracks.readTracks();
    auto * game = new MicroMachinesTh();
    auto * gameLoop = new GameLoopTh(*game);
    player->sendAllTrackNames(tracks.serialize());
    player->setMatch();
    player->setCar(game->getNextCar());
    player->sendCarData();
    game->addPlayer(player);
    players[player] = gamesNumber;
    player->setState(waitingPlayers);

    games.emplace(gamesNumber, game);
    gameLoops.emplace(gamesNumber, gameLoop);
    games[gamesNumber]->start();
    gameLoops[gamesNumber]->start();
    gamesNumber++;
}

// Adds player to existent match. Receives match sent by user in setMatch()
void GamesTh::addPlayer(ClientTh *player) {
    int gameIndex = -1;
    player->setAvailableGames(serializeGames());
    player->sendAvailableGames();
    player->setMatch();
    gameIndex = player->getGameNumber();
    player->setCar(games[gameIndex]->getNextCar());
    games[gameIndex]->addPlayer(player);
    players[player] = gameIndex;
    player->sendTrackData(games[gameIndex]->trackSerialized());
    player->sendCarData();
    player->setState(waitingPlayers);
}

// Sends available games
std::string GamesTh::serializeGames() {
    std::string gamesStr{};
    for (auto game : games)
        gamesStr += std::to_string(game.first) + ",";
    gamesStr.back() = '\n';
    return gamesStr;
}

// Adds player sent by acceptorThread to a list for processing it.
void GamesTh::setPlayerToAssign(ClientTh * player) {
    players.emplace(player, PLAYERTOASSIGN);
}

void GamesTh::removePlayer(ClientTh *player, int gameIndex) {
    games[gameIndex]->removePlayer(player);
}

// Tells where a player is playing
int GamesTh::getPlayerGameID(ClientTh* player) {
    return players[player];
}

// Removes all players from game
void GamesTh::cleanPlayers(int gameIndex) {
    games[gameIndex]->cleanPlayers();
}

// Tells how many games started
int GamesTh::getGamesNumber() {
    return gamesNumber;
}
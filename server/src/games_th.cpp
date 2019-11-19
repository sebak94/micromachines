//
// Created by fperezboco on 17/11/19.
//

#include "../include/games_th.h"
#include "../include/model/micromachines_th.h"
#include "../include/game_loop_th.h"

void GamesTh::run() {
    while (running) {
        for (auto player : players) {

            if (player.second == -1) {
                if (player.first->getState() == mainMenu) {
                } else if (player.first->getState() == creating) {
                    createGame(player.first);
                } else if (player.first->getState() == joining) {
                    addPlayer(player.first);
                }
            }
         }
    }
}

void GamesTh::stop() {
    running = false;
}

int GamesTh::createGame(ClientTh *player) {
    auto * game = new MicroMachinesTh();
    auto * gameLoop = new GameLoopTh(*game);
    games.emplace(gamesNumber, game);
    gameLoops.emplace(gamesNumber, gameLoop);
    TrackList tracks;
    tracks.readTracks();
    player->sendAllTrackNames(tracks.serialize());
    player->setMatch();
    player->setCar(game->getNextCar());
    player->sendCarData();
    game->addPlayer(player);
    players[player] = gamesNumber;
    player->setState(waitingPlayers);

    games[gamesNumber]->start();
    gameLoops[gamesNumber]->start();
    return gamesNumber++;
}

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

std::string GamesTh::serializeGames() {
    std::string gamesStr{};
    for (auto game : games)
        gamesStr += std::to_string(game.first) + ",";
    gamesStr.back() = '\n';
    return gamesStr;
}

void GamesTh::setPlayerOnMainMenu(ClientTh * player) {
    players.emplace(player, -1);
}

void GamesTh::removePlayer(ClientTh *player, int gameIndex) {
    games[gameIndex]->removePlayer(player);
}

int GamesTh::getPlayerGameID(ClientTh* player) {
    return players[player];
}

void GamesTh::cleanPlayers(int gameIndex) {
    games[gameIndex]->cleanPlayers();
}

int GamesTh::getGamesNumber() {
    return gamesNumber;
}

std::string GamesTh::trackSerialized(int gameIndex) {
    return games[gameIndex]->trackSerialized();
}

std::string GamesTh::lapsSerialized(int gameIndex) {
    return games[gameIndex]->lapsSerialized();
}

std::string GamesTh::allTrackNames(int gameIndex) {
    return games[gameIndex]->allTrackNames();
}
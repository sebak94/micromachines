//
// Created by fperezboco on 21/11/19.
//

#ifndef MICROMACHINES_GAMESTATE_H
#define MICROMACHINES_GAMESTATE_H

typedef enum {
    mainMenu,
    creating,
    joining,
    waitingPlayers,
    startCountdown,
    playing,
    waitingEnd,
    gameEnded
} GameState;

#endif //MICROMACHINES_GAMESTATE_H

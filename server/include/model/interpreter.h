//
// Created by mati on 11/1/19.
//

#ifndef MICROMACHINES_INTERPRETER_H
#define MICROMACHINES_INTERPRETER_H


#include "../../include/model/cars/states/car_state.h"
#include "../../include/model/cars/states/accelerate.h"
#include "../../include/model/cars/states/left.h"
#include "../../include/model/cars/states/right.h"
#include "../../include/model/cars/states/break.h"
#include "../../include/player_action.h"


class Interpreter {
public:
    CarState *interpret(PlayerAction pa);
};


#endif //MICROMACHINES_INTERPRETER_H

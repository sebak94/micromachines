//
// Created by mati on 11/1/19.
//

#ifndef MICROMACHINES_INTERPRETER_H
#define MICROMACHINES_INTERPRETER_H


#include "../../include/model/cars/states/car_state.h"
#include <memory>

class Interpreter {
public:
    std::shared_ptr<CarState> interpret(char action);
};


#endif //MICROMACHINES_INTERPRETER_H

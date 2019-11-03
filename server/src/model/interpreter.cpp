//
// Created by mati on 11/1/19.
//

#include "../../include/model/interpreter.h"
#include "../../include/model/cars/states/accelerate.h"
#include "../../include/model/cars/states/left.h"
#include "../../include/model/cars/states/right.h"
#include "../../include/model/cars/states/break.h"
#include <memory>
#include "iostream"

std::shared_ptr<CarState> Interpreter::interpret(char action) {
    switch (action) {
        case 'A': return std::shared_ptr<Accelerate>(new Accelerate());
        case 'L': return std::shared_ptr<Left>(new Left());
        case 'R': return std::shared_ptr<Right>(new Right());
        case 'B': return std::shared_ptr<Break>(new Break());
        default: std::cout << "Accion inválida\n";
    }
}

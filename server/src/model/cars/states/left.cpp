#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <stdio.h>
#include <memory>

CarState* Left::newState(Break *previous) {
    return new Left();
}

CarState* Left::newState(Left *previous) {
    return new Left();
}

CarState* Left::newState(Right *previous) {
    return new Left();
}

CarState* Left::newState(Accelerate *previous) {
    return new Left();
}

void Left::apply(Car *car) {

}

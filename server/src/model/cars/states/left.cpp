#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <stdio.h>
#include <memory>

CarState* Left::newState(Break *previous) {
    Left l;
    return std::move(&l);
}

CarState* Left::newState(Left *previous) {
    Left l;
    return std::move(&l);
}

CarState* Left::newState(Right *previous) {
    Left l;
    return std::move(&l);
}

CarState* Left::newState(Accelerate *previous) {
    Left l;
    return std::move(&l);
}

void Left::apply(Car *car) {

}

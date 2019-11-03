#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <memory>
#include <stdio.h>

CarState* Accelerate::newState(Break *previous) {
    return new Accelerate();
}

CarState* Accelerate::newState(Left *previous) {
    return new Accelerate();
}

CarState* Accelerate::newState(Right *previous) {
    return new Accelerate();
}

CarState* Accelerate::newState(Accelerate *previous) {
    return new Accelerate();
}

void Accelerate::apply(Car *car) {
    // printf("Update car\n");
}

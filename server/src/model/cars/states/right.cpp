#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <stdio.h>
#include <memory>

CarState* Right::newState(Break *previous) {
    return new Right();
}

CarState* Right::newState(Left *previous) {
    return new Right();
}

CarState* Right::newState(Right *previous) {
    return new Right();
}

CarState* Right::newState(Accelerate *previous) {
    return new Right();
}

void Right::apply(Car *car) {

}

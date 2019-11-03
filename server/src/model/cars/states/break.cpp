#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <stdio.h>
#include <memory>

CarState* Break::newState(Break *previous) {
    return new Break;
}

CarState* Break::newState(Left *previous) {
    return new Break;
}

CarState* Break::newState(Right *previous) {
    return new Break;
}

CarState* Break::newState(Accelerate *previous) {
    return new Break;
}

void Break::apply(Car *car) {

}

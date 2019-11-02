#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <stdio.h>
#include <memory>

CarState* Break::newState(Break *previous) {
    Break b;
    return std::move(&b);
}

CarState* Break::newState(Left *previous) {
    Break b;
    return std::move(&b);
}

CarState* Break::newState(Right *previous) {
    Break b;
    return std::move(&b);
}

CarState* Break::newState(Accelerate *previous) {
    Break b;
    return std::move(&b);
}

void Break::apply(Car *car) {

}

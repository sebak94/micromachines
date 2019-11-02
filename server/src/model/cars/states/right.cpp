#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <stdio.h>
#include <memory>

CarState* Right::newState(Break *previous) {
    Right r;
    return std::move(&r);
}

CarState* Right::newState(Left *previous) {
    Right r;
    return std::move(&r);
}

CarState* Right::newState(Right *previous) {
    Right r;
    return std::move(&r);
}

CarState* Right::newState(Accelerate *previous) {
    Right r;
    return std::move(&r);
}

void Right::apply(Car *car) {

}

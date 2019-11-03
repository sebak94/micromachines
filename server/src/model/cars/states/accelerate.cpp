#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <memory>
#include <stdio.h>

CarState* Accelerate::newState(Break *previous) {
    Accelerate ac;
    return std::move(&ac);
}

CarState* Accelerate::newState(Left *previous) {
    Accelerate ac;
    return std::move(&ac);
}

CarState* Accelerate::newState(Right *previous) {
    Accelerate ac;
    return std::move(&ac);
}

CarState* Accelerate::newState(Accelerate *previous) {
    printf("Accelerate::newState(Accelerate *previous)\n");
    Accelerate ac;
    return std::move(&ac);
}

void Accelerate::apply(Car *car) {

}

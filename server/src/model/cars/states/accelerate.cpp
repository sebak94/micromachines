#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"
#include <memory>

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
    Accelerate ac;
    return std::move(&ac);
}

void Accelerate::apply(Car *car) {

}

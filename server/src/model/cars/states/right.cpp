#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/car.h"

void Right::apply(Car *car) {
    car->addRotation(10);
}

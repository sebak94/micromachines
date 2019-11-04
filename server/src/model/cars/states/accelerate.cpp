#include "../../../../include/model/cars/states/accelerate.h"
#include "../../../../include/model/cars/car.h"

void Accelerate::apply(Car *car) {
    car->addPositionX(2);
}

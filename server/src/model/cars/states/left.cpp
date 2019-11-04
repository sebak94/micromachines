#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/car.h"

void Left::apply(Car *car) {
    car->addRotation(-10);
}

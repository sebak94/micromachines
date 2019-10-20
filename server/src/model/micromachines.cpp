#include "../../include/model/micromachines.h"
#include "../../include/model/car.h"

void Micromachines::update() {
    for (size_t i = 0; i < cars.size(); i++) {
        cars[i].update();
    }
}

void Micromachines::addCar(Car &car) {
    cars.push_back(car);
}

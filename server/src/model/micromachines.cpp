#include "../../include/model/micromachines.h"
#include "../../include/model/cars/car.h"

void Micromachines::update() {
    for (size_t i = 0; i < cars.size(); i++) {
        cars[i]->update();
    }
}

void Micromachines::addCar(Car *car) {
    cars.push_back(car);
}

size_t Micromachines::removeCar(Car *car) {
    size_t index_to_remove = -1;

    for (size_t i = 0; i < cars.size(); i++) {
        if (cars[i] == car) {
            index_to_remove = i;
        }
    }

    if (index_to_remove != -1) {
        cars.erase(cars.begin() + index_to_remove);
    }
    
    return index_to_remove;
}

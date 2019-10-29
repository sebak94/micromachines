#include "../../../include/model/cars/blue_car.h"
#include "../../../include/model/point.h"

BlueCar::BlueCar(): Car(100, 40, 100, 25, 2, 2, Point(0, 0), blue) {}

void BlueCar::update() {
    state->apply(this);
}

void BlueCar::serialize() {}

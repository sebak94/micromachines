#include "../../../include/model/cars/black_car.h"

BlackCar::BlackCar(b2World *world): Car(100, 40, 250, 2, 2, 2, Point(50, 100),
    black, world) {}

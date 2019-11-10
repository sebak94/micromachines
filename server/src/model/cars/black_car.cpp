#include "../../../include/model/cars/black_car.h"

BlackCar::BlackCar(b2World *world, const Point &startingPoint, uint16_t rot) :
    Car(100, 40, 150, 2, 2, 2, startingPoint, black, rot, world) {}

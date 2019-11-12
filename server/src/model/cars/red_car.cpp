#include "../../../include/model/cars/red_car.h"

RedCar::RedCar(b2World *world, const Point &startingPoint, uint16_t rot) :
    Car(100, 40, 150, 2, 2, 2, startingPoint, red, rot, world) {}

#include "../../../include/model/cars/yellow_car.h"

YellowCar::YellowCar(b2World *world, const Point &startingPoint, uint16_t rot) :
    Car(100, 40, 150, 2, 2, 2, startingPoint, yellow, rot, world) {}

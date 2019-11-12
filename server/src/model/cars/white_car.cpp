#include "../../../include/model/cars/white_car.h"

WhiteCar::WhiteCar(b2World *world, const Point &startingPoint, uint16_t rot) :
    Car(100, 40, 150, 2, 2, 2, startingPoint, white, rot, world) {}

#include "../../../include/model/cars/blue_car.h"

BlueCar::BlueCar(b2World *world, const Point &startPoint, uint16_t rot,
                 int startID) :
        Car(100, 40, 150, 2, 2, 2, startPoint, blue, rot, world, startID) {}

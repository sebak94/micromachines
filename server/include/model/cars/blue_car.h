#ifndef __BLUE_CAR_H__
#define __BLUE_CAR_H__

#include "car.h"

class BlueCar: public Car {
    public:
    BlueCar(b2World *world, const Point &startPoint, uint16_t rot,
            int startID);
    ~BlueCar();
};

#endif

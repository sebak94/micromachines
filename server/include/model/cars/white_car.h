#ifndef __WHITE_CAR_H__
#define __WHITE_CAR_H__

#include "car.h"

class WhiteCar: public Car {
    public:
    WhiteCar(b2World *world, const Point &startingPoint, uint16_t rot,
             int startID);
    ~WhiteCar();
};

#endif

#ifndef __RED_CAR_H__
#define __RED_CAR_H__

#include "car.h"

class RedCar : public Car {
public:
    RedCar(b2World *world, const Point &startingPoint, uint16_t rot,
           int startID);
    ~RedCar();
};

#endif

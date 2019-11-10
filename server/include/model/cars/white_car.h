#ifndef __WHITE_CAR_H__
#define __WHITE_CAR_H__

#include "car.h"

class WhiteCar: public Car {
    public:
    WhiteCar(const Point &startingPoint, uint16_t rot);
    ~WhiteCar();
};

#endif

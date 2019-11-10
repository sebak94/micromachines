#ifndef __RED_CAR_H__
#define __RED_CAR_H__

#include "car.h"

class RedCar : public Car {
public:
    RedCar(const Point &startingPoint, uint16_t rot);
    ~RedCar();
};

#endif

#ifndef __BLACK_CAR_H__
#define __BLACK_CAR_H__

#include "car.h"

class BlackCar: public Car {
    public:
    BlackCar(b2World *world);
    ~BlackCar();
};

#endif

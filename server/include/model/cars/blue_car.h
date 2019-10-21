#ifndef __BLUE_CAR_H__
#define __BLUE_CAR_H__

#include "car.h"

class BlueCar: public Car {
    public:
    BlueCar();
    virtual void update() override;
    virtual void serialize() override;
    ~BlueCar();
};

#endif

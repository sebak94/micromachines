#ifndef __ACCELERATE_H__
#define __ACCELERATE_H__

#include "car_state.h"

class Car;

class Accelerate : public CarState {
    public:
    virtual void apply(Car *car) override;
};

#endif

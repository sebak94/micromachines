#ifndef __LEFT_H__
#define __LEFT_H__

#include "car_state.h"

class Car;

class Left: public CarState {
    public:
    virtual void apply(Car *car) override;
};

#endif

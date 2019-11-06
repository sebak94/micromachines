#ifndef __RIGHT_H__
#define __RIGHT_H__

#include "car_state.h"

class Car;

class Right: public CarState {
    public:
    virtual void apply(Car *car) override;
};

#endif

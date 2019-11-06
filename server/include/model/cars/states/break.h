#ifndef __BREAK_H__
#define __BREAK_H__

#include "car_state.h"

class Car;

class Break: public CarState {
    public:
    virtual void apply(Car *car) override;
};

#endif

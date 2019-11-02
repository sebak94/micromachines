#ifndef __BREAK_H__
#define __BREAK_H__

#include "car_state.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/accelerate.h"

class Car;

class Break: public CarState {
    public:
    virtual CarState* newState(Break *previous) override;
    virtual CarState* newState(Left *previous) override;
    virtual CarState* newState(Right *previous) override;
    virtual CarState* newState(Accelerate *previous) override;
    virtual void apply(Car *car) override;
};

#endif

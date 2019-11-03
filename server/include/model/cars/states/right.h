#ifndef __RIGHT_H__
#define __RIGHT_H__

#include "car_state.h"
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/break.h"

class Car;

class Right: public CarState {
    public:
    virtual CarState* newState(Break *previous) override;
    virtual CarState* newState(Left *previous) override;
    virtual CarState* newState(Right *previous) override;
    virtual CarState* newState(Accelerate *previous) override;
    virtual void apply(Car *car) override;
};

#endif
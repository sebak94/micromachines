#ifndef __CAR_STATE_H__
#define __CAR_STATE_H__

class Break;
class Left;
class Right;
class Accelerate;
class Car;

class CarState {
    public:
    virtual CarState* newState(Break *previous) = 0;
    virtual CarState* newState(Left *previous) = 0;
    virtual CarState* newState(Right *previous) = 0;
    virtual CarState* newState(Accelerate *previous) = 0;
    virtual void apply(Car *car) = 0;
};

#endif

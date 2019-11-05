#ifndef __CAR_STATE_H__
#define __CAR_STATE_H__

class Car;

class CarState {
    public:
    virtual void apply(Car *car) = 0;
};

#endif

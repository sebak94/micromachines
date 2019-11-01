#ifndef __CAR_STATE_H__
#define __CAR_STATE_H__

class Car;

class CarState {
    public:
    virtual void apply(const CarState &previous) = 0;
};

#endif

#ifndef __CAR_STATES_H__
#define __CAR_STATES_H__

#include "../cars/car.h"

class CarState {
    public:
    virtual void apply(Car &car) = 0; 
};

#endif

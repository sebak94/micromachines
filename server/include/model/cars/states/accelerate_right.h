#ifndef __ACCELERATE_RIGHT_H__
#define __ACCELERATE_RIGHT_H__

#include "car_state.h"

class AccelerateRight: public CarState {
    public:
    virtual void apply(Car &car) override;
};

#endif

#ifndef __ACCELERATE_LFET_H__
#define __ACCELERATE_LFET_H__

#include "car_state.h"

class AccelerateLeft: public CarState {
    public:
    virtual void apply(Car *car) override;
};

#endif

#ifndef __NOTHING_H__
#define __NOTHING_H__

#include "car_state.h"

class Nothing: public CarState {
    public:
    virtual void apply(Car *car) override;
};

#endif

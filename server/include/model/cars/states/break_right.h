#ifndef __BREAK_RIGHT_H__
#define __BREAK_RIGHT_H__

#include "car_state.h"

class BreakRight: public CarState {
    public:
    virtual void apply(Car *car) override;
};

#endif

#ifndef __BREAK_LFET_H__
#define __BREAK_LFET_H__

#include "car_state.h"

class BreakLeft: public CarState {
    public:
    virtual void apply(Car &car) override;
};

#endif

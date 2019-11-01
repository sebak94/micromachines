#ifndef __ACCELERATE_H__
#define __ACCELERATE_H__

#include "car_state.h"
#include <utility>
#include <cstdio>
#include "../../../../include/model/cars/states/right.h"
#include "../../../../include/model/cars/states/left.h"
#include "../../../../include/model/cars/states/break.h"
#include "../../../../include/model/cars/car.h"

class Accelerate : public CarState {
public:
    void apply(const Break &previous);

    void apply(const Left &previous);

    void apply(const Right &previous);

    void apply(const Accelerate &previous);

    void apply(const CarState &previous) override;
};

#endif

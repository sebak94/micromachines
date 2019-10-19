#ifndef __MICROMACHINES_H__
#define __MICROMACHINES_H__

#include "./model/car.h"
#include <vector>

class Micromachines {
    private:
    std::vector<Car> cars;

    public:
    void update();
};

#endif

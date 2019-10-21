#ifndef __MICROMACHINES_H__
#define __MICROMACHINES_H__

#include "cars/car.h"
#include "track.h"
#include <vector>

class Micromachines {
    private:
    Track track;
    std::vector<Car*> cars;

    public:
    void update();
    void addCar(Car *car);
    size_t removeCar(Car *car);
};

#endif

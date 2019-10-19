#include "../include/micromachines.h"

void Micromachines::update() {
    for (size_t i = 0; i < cars.size(); i++) {
        cars[i].update();
    }
}

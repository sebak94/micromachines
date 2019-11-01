#include "../../../../include/model/cars/states/accelerate.h"

void Accelerate::apply(const Break &previous) {
    printf("Break\n");
}

void Accelerate::apply(const Left &previous) {
    printf("Acelerate left\n");
}

void Accelerate::apply(const Right &previous) {
    printf("Acelerate Right\n");
}

void Accelerate::apply(const Accelerate &previous) {
    printf("acelerate\n");
}

void Accelerate::apply(const CarState &previous) {

}


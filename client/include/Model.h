#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "Block.h"
#include "Car.h"

class Model {
private:
    std::vector<Block> blocks;
    std::vector<Car> cars;

public:
    Model(int cantBlocks);
    ~Model();
    std::vector<Block>& getBlocks();
    std::vector<Car>& getCars();
};

#endif

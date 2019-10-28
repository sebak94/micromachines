#include "../include/Model.h"

Model::Model(int cantBlocks) {
    //Invento una pista
    Block block1(0, 0, PicType::CURVE_LEFT_TO_UP);
    Block block2(0, 100, PicType::LINE_VERTICAL_UP);
    Block block3(0, 200, PicType::CURVE_UP_TO_RIGHT);
    Block block4(100, 200, PicType::LINE_HORIZONTAL_RIGHT);
    Block block5(200, 200, PicType::CURVE_RIGHT_TO_DOWN);
    Block block6(200, 100, PicType::LINE_VERTICAL_DOWN);
    Block block7(200, 0, PicType::CURVE_DOWN_TO_LEFT);
    Block block8(100, 0, PicType::LINE_HORIZONTAL_LEFT);
    blocks.push_back(block1);
    blocks.push_back(block2);
    blocks.push_back(block3);
    blocks.push_back(block4);
    blocks.push_back(block5);
    blocks.push_back(block6);
    blocks.push_back(block7);
    blocks.push_back(block8);

    Car car1(50, 250, 45, 100, PicType::CAR_RED);
    Car car2(50, 150, 0, 100, PicType::CAR_WHITE);
    cars.push_back(car1);
    cars.push_back(car2);
}

Model::~Model() {

}

std::vector<Block>& Model::getBlocks() {
    return this->blocks;
}

std::vector<Car>& Model::getCars() {
    return this->cars;
}

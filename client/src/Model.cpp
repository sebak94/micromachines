#include "../include/Model.h"

Model::Model() {
    Car car1(150, 100, 45, 100, PicType::CAR_RED);
    Car car2(250, 100, 0, 100, PicType::CAR_WHITE);
    cars.push_back(car1);
    cars.push_back(car2);
}

Model::~Model() {

}

std::vector<Car>& Model::getCars() {
    return this->cars;
}

void Model::setTrackPartData(std::vector<TrackPartData> trackPartData) {
    this->trackPartData = trackPartData;
}

std::vector<TrackPartData> Model::getTrackPartData() const {
    return this->trackPartData;
}

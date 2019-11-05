#include "../include/Model.h"

Model::Model() {

}

Model::~Model() {
    for (const auto pair : cars) {
        delete pair.second;
    }
}

std::map<std::string, Car*>& Model::getCars() {
    return this->cars;
}

void Model::setTrackPartData(std::vector<TrackPartData> trackPartData) {
    this->trackPartData = trackPartData;
}

std::vector<TrackPartData> Model::getTrackPartData() const {
    return this->trackPartData;
}

void Model::addCar(std::string str) {
    //serializacion: current_velocity, health, rotation, x, y, color\n
    size_t pos = 0;
    int velocity = std::stoi(parse(str, pos, ','));
    int health = std::stoi(parse(str, pos, ','));
    int rotation = std::stoi(parse(str, pos, ','));
    int x = std::stoi(parse(str, pos, ','));
    int y = std::stoi(parse(str, pos, ','));
    std::string color = parse(str, pos, '\n');
    cars[color] = new Car(x, y, rotation, health, color);
}

std::string Model::parse(const std::string &str, size_t &pos, const char delim) {
    std::string substr;
    size_t nextPos = str.find(delim, pos);
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

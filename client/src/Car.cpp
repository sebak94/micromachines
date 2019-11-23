#include "../include/Car.h"

Car::Car(std::string carStr) {
    setCarParams(carStr);
}

Car::~Car() {}

void Car::setCarParams(std::string carStr) {
    //serializacion: current_velocity, health, rotation, x, y, laps, color\n
    size_t pos = 0;
    parse(carStr, pos, ','); //velocidad
    this->health = std::stoi(parse(carStr, pos, ','));
    this->degrees = std::stoi(parse(carStr, pos, ','));
    this->x = std::stoi(parse(carStr, pos, ','));
    this->y = std::stoi(parse(carStr, pos, ','));
    this->laps = std::stoi(parse(carStr, pos, ','));
    this->color = parse(carStr, pos, '\n');
}

std::string Car::parse(const std::string &str, size_t &pos, const char delim) {
    std::string substr;
    size_t nextPos = str.find(delim, pos);
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

int Car::getX() const {
    return this->x;
}

int Car::getY() const {
    return this->y;
}

int Car::getDegrees() const {
    return this->degrees;
}

int Car::getHealth() const {
    return this->health;
}

int Car::getMyLap() const {
    return this->laps;
}

bool Car::collided() {
    if (collision) {
        collision = false;
        return true;
    } else
        return false;
}

std::string Car::getMyColor() const {
    return this->color;
}

void Car::update(std::string carStr) {
    setCarParams(carStr);

    if (health != lastHealth) {
        lastHealth = health;
        collision = true;
    }
}

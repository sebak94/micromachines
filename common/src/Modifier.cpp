#include "../include/Modifier.h"

Modifier::Modifier(int x, int y, std::string type, direction dir) {
    this->x = x;
    this->y = y;
    setType(type);
    this->dir = dir;
}

Modifier::~Modifier() {}

void Modifier::setType(std::string str) {
    if (str == "healthBox") this->type = healthBox;
    else if (str == "boost") this->type = boost;
    else if (str == "stones") this->type = stones;
    else if (str == "oil") this->type = oil;
    else if (str == "mud") this->type = mud;
}

std::string Modifier::getType() const {
    if (this->type == healthBox) return "healthBox";
    else if (this->type == boost) return "boost";
    else if (this->type == stones) return "stones";
    else if (this->type == oil) return "oil";
    else if (this->type == mud) return "mud";
}

int Modifier::getX() const {
    return x;
}

int Modifier::getY() const {
    return y;
}

std::string Modifier::serialize() {
    std::string resp;
    resp = std::to_string(x) + ',' + std::to_string(y) + ',' + getType();
    return resp;
}

void Modifier::travel(int step) {
    switch(dir) {
        case left: x -= step; break;
        case right: x += step; break;
        case up: y += step; break;
        case down: y -= step; break;
    }
}
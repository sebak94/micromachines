#include "../include/Modifier.h"

Modifier::Modifier() {

}

void Modifier::setType(std::string str) {
    if (str == "healthBox") this->type = healthBox;
    else if (str == "boost") this->type = boost;
    else if (str == "stones") this->type = stones;
    else if (str == "oil") this->type = oil;
    else if (str == "mud") this->type = mud;
}

Modifier::Modifier(const std::string &str) {
    size_t pos = 0;
    this->x = std::stoi(parse(str, pos, ','));
    this->y = std::stoi(parse(str, pos, ','));
    std::string typeStr = parse(str, pos, '\n');
    setType(typeStr);
}

std::string Modifier::parse(const std::string &str, size_t &pos, const char delim) {
    std::string substr;
    size_t nextPos = str.find(delim, pos);
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

Modifier::~Modifier() {

}

int Modifier::getX() const {
    return x;
}

int Modifier::getY() const {
    return y;
}

std::string Modifier::getType() const {
    if (this->type == healthBox) return "healthBox";
    else if (this->type == boost) return "boost";
    else if (this->type == stones) return "stones";
    else if (this->type == oil) return "oil";
    else if (this->type == mud) return "mud";
}

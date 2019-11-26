#include "../include/ModifierList.h"

ModifierList::ModifierList() {
    std::string type = "boost";
    modifiers.emplace(0, Modifier(0, 0, type, left));
    distance.emplace(0, 150 - rand() % 75);
}

void ModifierList::append(int x, int y, std::string type, direction dir) {
    modifiers.emplace(N, Modifier(x, y, type, dir));
    distance.emplace(N, 150 - rand() % 75);
    N++;
}

void ModifierList::updateDistance() {
    for (auto & modifier : modifiers) {
        if (distance[modifier.first] > 0) {
            modifier.second.travel(MODIFIER_STEP);
            distance[modifier.first] -= MODIFIER_STEP;
        }
    }
}

ModifierList::ModifierList(std::string str) {
    size_t pos = 0;
    N = 0;
    parse(str, pos, ','); //salteo hasta la primer coma
    str.erase(str.length()-1); //borro el \n

    while (pos <= str.length()) {
        int x = std::stoi(parse(str, pos, ','));
        int y = std::stoi(parse(str, pos, ','));
        std::string type = parse(str, pos, ',');
        Modifier modifier(x, y, type, left);
        modifiers.emplace(N, modifier);
        N++;
        if (pos == 0)
            break;
    }
}

ModifierList::~ModifierList() {

}

std::string ModifierList::serialize() {
    std::string resp{};
    //resp = std::string("M,");
    for (auto it = modifiers.begin(); it!=modifiers.end(); it++) {
        resp += it->second.serialize();
        resp += ',';
    }
    resp.back() = '\n';
    return std::string("M,") + resp;
}

std::string ModifierList::parse(const std::string &str, size_t &pos, const char delim) {
    std::string substr;
    size_t nextPos = str.find(delim, pos);
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

std::vector<Modifier> ModifierList::getModifiers() {
    std::vector<Modifier> vect{};
    for (auto it = modifiers.begin(); it!=modifiers.end(); it++)
        vect.push_back(it->second);
    return vect;
}

bool ModifierList::isOnBoost(int posX, int posY) {
    return isOnModif(posX, posY, "boost");
}

bool ModifierList::isOnHealth(int posX, int posY) {
    return isOnModif(posX, posY, "healthBox");
}

bool ModifierList::isOnStones(int posX, int posY) {
    return isOnModif(posX, posY, "stones");
}

bool ModifierList::isOnOil(int posX, int posY) {
    return isOnModif(posX, posY, "oil");
}

bool ModifierList::isOnMud(int posX, int posY) {
    return isOnModif(posX, posY, "mud");
}

bool ModifierList::isOnModif(int & posX, int & posY, const std::string& type) {
    for (const auto& modifier : modifiers) {
        if (modifier.second.getType() == type) {
            if (posX >= modifier.second.getX()
                && posX <= modifier.second.getX() + MODIFIER_SIZE
                && posY >= modifier.second.getY()
                && posY <= modifier.second.getY() + MODIFIER_SIZE) {
                    eraseModifier(modifier.first);
                    return true;
            }
        }
    }
    return false;
}

void ModifierList::eraseModifier(int i) {
    modifiers.erase(i);
}

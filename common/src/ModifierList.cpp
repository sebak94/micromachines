#include "../include/ModifierList.h"

ModifierList::ModifierList() {
    Modifier modifier(900,500,"healthBox");
    Modifier modifier21(800,525,"boost");
    Modifier modifier22(825,525,"boost");
    Modifier modifier23(850,525,"boost");
    Modifier modifier24(875,525,"boost");
    Modifier modifier25(900,525,"boost");
    Modifier modifier26(925,525,"boost");
    Modifier modifier27(950,525,"boost");
    Modifier modifier28(975,525,"boost");
    Modifier modifier29(1000,525,"boost");
    Modifier modifier3(900,537,"stones");
    Modifier modifier4(800,500,"oil");
    Modifier modifier5(800,537,"mud");

    modifiers.push_back(modifier);
    modifiers.push_back(modifier21);
    modifiers.push_back(modifier22);
    modifiers.push_back(modifier23);
    modifiers.push_back(modifier24);
    modifiers.push_back(modifier25);
    modifiers.push_back(modifier26);
    modifiers.push_back(modifier27);
    modifiers.push_back(modifier28);
    modifiers.push_back(modifier29);
    modifiers.push_back(modifier3);
    modifiers.push_back(modifier4);
    modifiers.push_back(modifier5);
}

ModifierList::ModifierList(std::string str) {
    size_t pos = 0;
    parse(str, pos, ','); //salteo hasta la primer coma
    str.erase(str.length()-1); //borro el \n

    while (pos <= str.length()) {
        int x = std::stoi(parse(str, pos, ','));
        int y = std::stoi(parse(str, pos, ','));
        std::string type = parse(str, pos, ',');
        Modifier modifier(x, y, type);
        modifiers.push_back(modifier);
        if (pos == 0)
            break;
    }
}

ModifierList::~ModifierList() {

}

std::string ModifierList::serialize() {
    std::string resp = "M,";
    for (int i = 0; i < modifiers.size(); i++) {
        resp += modifiers[i].serialize();
        resp += ',';
    }
    resp.erase(resp.length()-1);
    resp += '\n';
    return resp;
}

std::string ModifierList::parse(const std::string &str, size_t &pos, const char delim) {
    std::string substr;
    size_t nextPos = str.find(delim, pos);
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

std::vector<Modifier>& ModifierList::getModifiers() {
    return modifiers;
}

bool ModifierList::isOnBoost(int posX, int posY) {
    bool result = false;
    for (auto modifier : modifiers) {
        if (modifier.getType() == "boost") {
            if (posX >= modifier.getX()
                && posX <= modifier.getX() + MODIFIER_SIZE
                && posY >= modifier.getY()
                && posY <= modifier.getY() + MODIFIER_SIZE
            )
                result = true;
        }
    }
    return result;
}

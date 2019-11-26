//
// Created by mati on 11/11/19.
//

#include "../../include/lua/ai.h"


Ai::Ai() {
    this->L = lua_initialize();
    lua_open_files(this->L);
}

void Ai::setColor(std::string &color) {
    this->carColor = color;
}

void Ai::setTrack(std::vector <TrackPartData> track) {
    const char *matrix[20][20];

    for (int i = 0; i <= 15; i++) {
        for (int j = 0; j <= 15; j++) {
            matrix[i][j] = "empty";
        }
    }

    for (auto &i : track) {
        int x = i.getPosX() / 100;
        int y = i.getPosY() / 100;
        matrix[x][y] = get_enum_as_char(i.getType());
    }

    lua_load_map(this->L, matrix);
    printf("LOAD OK\n");
}


const char *Ai::get_enum_as_char(trackPartType type) {
    switch (type) {
        case empty :
            return "empty";
        case downRight :
            return "downRight";
        case downLeft :
            return "downLeft";
        case upRight :
            return "upRight";
        case upLeft :
            return "upLeft";
        case horizontal :
            return "horizontal";
        case vertical :
            return "vertical";
        case finishH :
            return "finishH";
        case finishV :
            return "finishV";
        case public1Up :
            return "public1Up";
        case public1Down :
            return "public1Down";
        case public1Left :
            return "public1Left";
        case public1Right :
            return "public1Right";
        default :
            return "empty";
    }
}

const char *Ai::get_next_move(std::map<std::string, Car *> cars) {
    for (auto &it : cars) {
        Car *car = it.second;
        if (car->getMyColor() == this->carColor)
            return lua_get_next_movement(this->L, car->getX(), car->getY());
    }
    return "";
}

Ai::~Ai() {
    lua_close_file(L);
}

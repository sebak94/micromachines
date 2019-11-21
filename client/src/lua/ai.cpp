//
// Created by mati on 11/11/19.
//

#include "../../include/lua/ai.h"


Ai::Ai() {
    this->L = lua_initialize();
    lua_open_files(this->L);

}

void Ai::setTrack(std::vector <TrackPartData> &track) {
    const char *matrix[100][100];
    printf("tracksize:%i\n", track.size());
    for (auto &i : track) {
        matrix[i.getPosX()][i.getPosY()] = get_enum_as_char(i.getType());
        printf("%i,%i:%s\n", i.getPosX(), i.getPosY(), matrix[i.getPosX()][i
                .getPosY()]);
    }

    //lua_load_map(this->L, matrix);
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
    }
}

Ai::~Ai() {
    lua_close_file(L);
}

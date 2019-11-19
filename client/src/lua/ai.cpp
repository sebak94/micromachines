//
// Created by mati on 11/11/19.
//

#include "../../include/lua/ai.h"


Ai::Ai(std::vector <TrackPartData> &track) {
    this->L = lua_initialize();
    lua_open_files(this->L);
}

Ai::~Ai() {
    lua_close_file(L);
}

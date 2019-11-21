//
// Created by mati on 11/11/19.
//

#ifndef MICROMACHINES_AI_H
#define MICROMACHINES_AI_H


#ifdef __cplusplus
// Este header es básicamente una inclusión de los 3 de abajo
// pero protegido con extern "C"
#include "lua.hpp"
#else

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#endif
extern "C" {
lua_State *lua_initialize();
int lua_open_files(lua_State *L);
void lua_close_file(lua_State *L);
void lua_load_map(lua_State *L, const char *track[100][100]);
int lua_get_next_movement(lua_State *L, int positionX, int positionY);
void lua_play(lua_State *L);
}

#include "../../../common/include/TrackPartData.h"
#include <vector>

class Ai {
private:
    lua_State *L;

public:
    Ai();

    void setTrack(std::vector <TrackPartData> &track);

    const char *get_enum_as_char(trackPartType type);

    ~Ai();
};


#endif //MICROMACHINES_AI_H

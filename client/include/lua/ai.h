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

#include "../../../common/include/TrackPartData.h"
#include <vector>
#include <cstring>
#include <map>
#include "../Car.h"

extern "C" {
lua_State *lua_initialize();
int lua_open_files(lua_State *L);
void lua_close_file(lua_State *L);
void lua_load_map(lua_State *L, const char *matrix[20][20]);
const char *lua_get_next_movement(lua_State *L, int positionX, int positionY);
}

class Ai {
private:
    lua_State *L;
    std::string carColor = "white";

public:
    Ai();

    void setColor(std::string &color);

    void setTrack(std::vector <TrackPartData> &track);

    const char *get_enum_as_char(trackPartType type);

    const char *get_next_move(std::map<std::string, Car *> &cars);

    ~Ai();
};


#endif //MICROMACHINES_AI_H

//compliar: gcc -I/usr/include/lua5.3 -c ai.c -llua5.3 -lm

#ifdef __cplusplus
// Este header es básicamente una inclusión de los 3 de abajo
// pero protegido con extern "C"
#include "lua.hpp"
#else

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#endif

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define AI_PATH "../client/src/lua/ai.lua"

lua_State *lua_initialize() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    return L;
}

void lua_close_file(lua_State *L) {
    lua_close(L);
}

int lua_open_files(lua_State *L) {
    int result;
    result = luaL_dofile(L, AI_PATH);

    if (result != 0) {
        printf("Faltan los archivos lua:\n");
        printf("%s\n", AI_PATH);
    }
    return result;
}

void lua_load_map(lua_State *L, char *matrix[20][20]) {
    lua_newtable(L);
    for (int i = 0; i <= 15; i++) {
        lua_pushnumber(L, i);
        lua_newtable(L);
        for (int j = 0; j <= 15; j++) {
            lua_pushnumber(L, j);
            lua_pushstring(L, matrix[i][j]);
            lua_rawset(L, -3);
        }
        lua_rawset(L, -3);
    }
    lua_setglobal(L, "track_matrix");
}

const char *lua_get_next_movement(lua_State *L, int positionX, int positionY) {
    lua_getglobal(L, "getNextMove");
    lua_pushnumber(L, positionX / 100);
    lua_pushnumber(L, positionY / 100);
    lua_call(L, 2, 1);

    const char *nextMove;
    size_t strLen = 0;
    nextMove = lua_tolstring(L, 1, &strLen);
    lua_pop(L, 1);// Limpio el stack

    return nextMove;
}
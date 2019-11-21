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


void lua_load_map(lua_State *L, const char *track[100][100]) {
    lua_getglobal(L, "init_load_map");

    printf("INICIO\n");
    for (int i = 1; i < 99; i++) {
        for (int j = 1; j < 99; j++) {
            printf("%s\n", track[i][j]);
        }
    }
    printf("FIN\n");

    int result;
    lua_newtable(L);
    for (int i = 0; i < 3; i++) {
        lua_pushnumber(L, i + 1);    // parent table index
        lua_newtable(L);             // child table
        for (int j = 0; j < 3; j++) {
            lua_pushnumber(L, j + 1);  // this will be the child's index
            lua_pushstring(L, "asd");
            lua_settable(L, -3);
        }
        lua_settable(L, -3);
    }

    /* By what name is the script going to reference our table? */
    lua_setglobal(L, "foo");

    /* Ask Lua to run our little script */
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result)
        fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));

    lua_pop(L, 1);  /* Take the returned value out of the stack */
}


int lua_get_next_movement(lua_State *L, int positionX, int positionY) {
    lua_getglobal(L, "getNextMove");
    lua_pushnumber(L, positionX);
    lua_pushnumber(L, positionY);
    lua_call(L, 2, 1);
    const char *nextMove;
    size_t strLen = 0;
    nextMove = lua_tolstring(L, 1, &strLen);
    printf("El nuevo movimiento es: %s\n", nextMove);
    printf("strLen: %zu\n", strLen);
    // Limpio el stack
    lua_pop(L, 1);
    return 0;
}

void lua_play(lua_State *L) {
    for (int i = 1; i <= 2; ++i) {
        for (int j = 1; j <= 2; ++j) {
            lua_get_next_movement(L, i, j);
        }
    }
}

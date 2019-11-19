// Lua está pensado para trabajar con C, por lo cuál
// la biblioteca nos pide que nuestras funciones sean compatibles
// con la C, desactivando el name mangling
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

#define AI_PATH "../client/src/lua/movements.lua"
#define MOV_PATH "../client/src/lua/ai.lua"

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
    result = luaL_dofile(L, MOV_PATH);
    result = luaL_dofile(L, AI_PATH);

    if (result != 0) {
        printf("Faltan los archivos lua:\n");
        printf("%s\n", AI_PATH);
        printf("%s\n", MOV_PATH);
    }
    return result;
}

void lua_load_map(lua_State *L) {
    lua_getglobal(L, "init_load_map");

    lua_newtable(L);
    int top = lua_gettop(L);

    const char *key = "clave";
    const char *value = "value";
    lua_pushlstring(L, key, 6);
    lua_pushlstring(L, value, 6);
    lua_settable(L, top);

    printf("calling...\n");
    lua_call(L, 1, 0);
    printf("PASOO\n");
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

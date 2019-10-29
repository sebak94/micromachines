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

#define AI_PATH "ai.lua"
#define MOV_PATH "movements.lua"

int getNextPosition(lua_State *L, int positionX, int positionY);

int main(int argc, char *argv[]) {
    int result;
    char pwd_mv[1024];
    char pwd_ai[1024];

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    getcwd(pwd_mv, sizeof(pwd_mv));
    strcpy(pwd_ai, pwd_mv);
    strcat(pwd_mv, MOV_PATH);
    strcat(pwd_ai, AI_PATH);

    result = luaL_dofile(L, pwd_mv);
    result = luaL_dofile(L, pwd_ai);

    if (result == 0) {
        for (int i = 1; i <= 2; ++i) {
            for (int j = 1; j <= 2; ++j) {
                getNextPosition(L, i, j);
            }
        }
    } else {
        printf("Faltan los archivos lua:\n");
        printf("%s\n", pwd_mv);
        printf("%s\n", pwd_ai);
    }
    lua_close(L);
    return result;
}

int getNextPosition(lua_State *L, int positionX, int positionY) {
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
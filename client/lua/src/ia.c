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

#define IA_PATH "ia.lua"
#define MOV_PATH "movements.lua"

int getNextPosition(lua_State *L, int positionX, int positionY);

int main(int argc, char *argv[]) {
    int result;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    result = luaL_dofile(L, MOV_PATH);
    result = luaL_dofile(L, IA_PATH);
    if (result == 0) {
        result = getNextPosition(L, 0, 1);
    } else {
        printf("Faltan los archivos .lua\n");
        printf("%s\n", MOV_PATH);
        printf("%s\n", IA_PATH);
    }
    lua_close(L);
    return result;
}

int getNextPosition(lua_State *L, int positionX, int positionY) {
    // Chequeo el tamaño del stack de variables del motor de lua
    int stackSize = lua_gettop(L);
    printf("Tamaño del stack: %d\n", stackSize);
    // Agrego al stack la función a llamar
    lua_getglobal(L, "getNextMove");
    // Agrego al stack los parámetros de la función a llamar
    lua_pushnumber(L, positionX);
    lua_pushnumber(L, positionY);
    // Llamo a la función
    // lua_call(L, numParamsEntrada, numParamsSalida)
    // La función recibe 2 parámetros y devuelve 1
    lua_call(L, 2, 1);

    const char *nextMove;
    size_t strLen = 0;
    nextMove = lua_tolstring(L, 1, &strLen);
    printf("El nuevo movimiento es: %s\n", nextMove);
    printf("strLen: %zu\n", strLen);

    stackSize = lua_gettop(L);
    printf("Tamaño del stack post pushes: %d\n", stackSize);
    // Limpio el stack
    lua_pop(L, 1);
    // Verifico que efectivamente decrementó su tamaño
    stackSize = lua_gettop(L);
    printf("Tamaño final del stack: %d\n", stackSize);
    return 0;
}
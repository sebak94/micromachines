//
// Created by mati on 11/11/19.
//

#ifndef MICROMACHINES_AI_H
#define MICROMACHINES_AI_H

extern "C" {
#include "../../src/lua/ai.c"
}

class Ai {
private:
    lua_State *L;
public:
    Ai();

    int ai_open_files();

    ~Ai();
};


#endif //MICROMACHINES_AI_H

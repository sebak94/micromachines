//
// Created by mati on 11/11/19.
//

#include "../../include/lua/ai.h"


Ai::Ai() {
    this->L = initialize();
}

int Ai::ai_open_files(){
    open_files(this->L);
}

Ai::~Ai() {
    closeLua(L);
}

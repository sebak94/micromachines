//
// Created by fperezboco on 9/11/19.
//

#include "../include/Grandstand.h"

Grandstand::Grandstand(trackPartType type, int row, int col) {
    this->type = type;
    this->row = row;
    this->col = col;
    this->posX = BLOCKSIZE * col;
    this->posY = BLOCKSIZE * (height - row - 1);
}
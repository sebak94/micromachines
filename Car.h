//
// Created by fperezboco on 18/10/19.
//

#include "Error.h"

#ifndef TPFINAL_CAR_H
#define TPFINAL_CAR_H


class Car {
private:
    uint8_t x = 50;
    uint8_t y = 50;
    uint8_t w = 150;
    uint8_t h = 150;

public:

    SDL_Texture* carTexture = nullptr;

    void loadSprite(const std::string & img_path);
    ~Car();
    static SDL_Texture * loadTexture(const std::string & img_path, SDL_Renderer *gameRenderer);

    void move(int depth);

    void draw(SDL_Renderer *renderer);

    void move(int depth, const SDL_Event *e);
};


#endif //TPFINAL_CAR_H

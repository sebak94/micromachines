//
// Created by fperezboco on 18/10/19.
//

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_image.h>
#include "Car.h"

/*void Car::loadSprite(const std::string & img_path) {
    SDL_Surface* rawImage = nullptr;
    rawImage = IMG_Load(img_path.c_str() );
    Sprite = SDL_ConvertSurface( rawImage, gameSurface->format, 0 );
    //if(Sprite == nullptr )
    // throw
    SDL_FreeSurface( rawImage );
}*/

SDL_Texture * Car::loadTexture(const std::string & img_path, SDL_Renderer * gameRenderer) {
    SDL_Surface* rawImage = nullptr;
    SDL_Texture* tex = nullptr;
    rawImage = IMG_Load(img_path.c_str() );
    if(rawImage == nullptr )
        throw Error("Loading Image: %s.", img_path.c_str());
    tex = SDL_CreateTextureFromSurface(gameRenderer, rawImage);
    if(tex == nullptr )
        throw Error("Creating texture: %s.", img_path.c_str());
    SDL_FreeSurface( rawImage );
    return tex;
}

Car::~Car() {
    SDL_DestroyTexture(carTexture);
    carTexture = nullptr;
}

void Car::move(int depth, const SDL_Event * e) {
    if (e->type == SDL_KEYDOWN) {
        SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) *e;
        switch (keyEvent.keysym.sym) {
            case SDLK_LEFT:
                x -= depth;
                break;
            case SDLK_RIGHT:
                x += depth;
                break;
            case SDLK_UP:
                y -= depth;
                break;
            case SDLK_DOWN:
                y += depth;
                break;
        }
    }
}

void Car::draw(SDL_Renderer * renderer) {
    SDL_Rect dest_rect = { x, y, w, h };
    SDL_RenderCopy( renderer, carTexture, nullptr, &dest_rect );
}
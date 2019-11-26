//
// Created by fperezboco on 20/10/19.
//

#include <SDL2/SDL_system.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../include/Button.h"
#include "../../common/include/Error.h"


Button::Button() = default;

/*Button::~Button() {
    SDL_DestroyTexture(texture);
}*/

void Button::clean() {
    area = {};
    pressed = false;
    previousPressState = false;
    SDL_DestroyTexture(texture);
}

Button::Button(SDL_Renderer *renderer, SDL_Rect &area, const std::string &path) {
    this->area = {area.x, area.y, area.w, area.h};
    texture = loadTexture(path, renderer);
}

void Button::updatePos(int x, int y) {
    area.x = x, area.y = y;
}

void Button::updateSize(int w, int h) {
    area.w = w;
    area.h = h;
}

void Button::draw(SDL_Renderer * renderer) {
    uint8_t d = 200;  // darkness factor
    SDL_SetTextureAlphaMod(texture, pressed*d+(1-pressed)*255);
    SDL_RenderCopy( renderer, texture, nullptr, &area );
}

void Button::drawRotated(SDL_Renderer * renderer, double angle) {
    uint8_t d = 200;  // darkness factor
    SDL_SetTextureAlphaMod(texture, pressed*d+(1-pressed)*255);
    SDL_RenderCopyEx( renderer, texture, nullptr, &area, angle, nullptr, SDL_FLIP_NONE );

}

void Button::updateEvent(const SDL_Event * event){
    if(event->type == SDL_MOUSEBUTTONDOWN && 
        event->button.button == SDL_BUTTON_LEFT &&
        event->button.x >= area.x &&
        event->button.x <= (area.x + area.w) &&
        event->button.y >= area.y &&
        event->button.y <= (area.y + area.h)) {
            pressed = true;
    } else if (event->type == SDL_MOUSEBUTTONUP &&
               event->button.button == SDL_BUTTON_LEFT) {
        pressed = false;
    }
}

/* Returns true if button goes OFF->ON */
bool Button::isClicked() {
    bool clicked = (pressed != previousPressState && pressed);
    previousPressState = pressed;
    return clicked;
}

SDL_Texture * Button::loadTexture(const std::string & img_path, SDL_Renderer * renderer) {
        SDL_Surface* rawImage = IMG_Load(img_path.c_str());;
        SDL_Texture* tex = nullptr;
        if (rawImage == nullptr)
            throw Error("Loading Image: %s.", img_path.c_str());
        tex = SDL_CreateTextureFromSurface(renderer, rawImage);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        if (tex == nullptr )
            throw Error("Creating texture: %s.", img_path.c_str());
        SDL_FreeSurface(rawImage);
        return tex;
}

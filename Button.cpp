//
// Created by fperezboco on 20/10/19.
//

#include <SDL2/SDL_system.h>
#include <SDL2/SDL_events.h>
#include <iostream>
#include "Button.h"

Button::Button(uint8_t r,
               uint8_t g,
               uint8_t b,
               uint8_t a,
               SDL_Rect &area) {
    this->area.x = area.x;
    this->area.y = area.y;
    this->area.w = area.w;
    this->area.h = area.h;
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Button::draw(SDL_Renderer * renderer) {
    uint8_t d = pressed*50;  // darkness factor
    SDL_SetRenderDrawColor(renderer, r - d, g - d*(g >= d), b - d*(g >= d), a);
    SDL_RenderFillRect(renderer, &area);
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
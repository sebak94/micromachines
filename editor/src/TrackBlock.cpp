//
// Created by fperezboco on 4/11/19.
//

#include <SDL2/SDL_system.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../../common/include/Track.h"
#include "../../common/include/TrackPartData.h"
#include "../include/TrackBlock.h"
#include "../../common/include/Button.h"
#include "../include/Error.h"

/* Creates track block of specific area and applies specific texture */
TrackBlock::TrackBlock(SDL_Renderer *renderer, SDL_Rect &area, const std::string &path) {
    this->area = {area.x, area.y, area.w, area.h};
    texture = loadTexture(path, renderer);
}

/* Configs already existent block setting area, texture and type */
void TrackBlock::setBlock(SDL_Texture * tex, trackPartType t, int x, int y, int w, int h) {
    area = {x, y, w, h};
    pressed = false;
    previousPressState = false;
    texture = tex;
    type = t;
    fixedPosX = area.x;
    fixedPosY = area.y;
}

/* Draws 1 block. Lightens it if mouse is over it. */
void TrackBlock::draw(SDL_Renderer * renderer) {
    uint8_t d = SHADOW_FACTOR;  // shadow factor
    bool b = inMouseArea;
    SDL_SetTextureAlphaMod(texture, b*d + (1 - b)*255);
    SDL_RenderCopy(renderer, texture, nullptr, &area);
}

/* Updates block event. (Pressing, releasing, over it) */
void TrackBlock::updateEvent(const SDL_Event * event){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (event->type == SDL_MOUSEBUTTONDOWN &&
        isInMouseArea(event->button.x, event->button.y)) {
            pressed = true;
    } else if (event->type == SDL_MOUSEBUTTONUP &&
               event->button.button == SDL_BUTTON_LEFT) {
        pressed = false;
    } else inMouseArea = isInMouseArea(x, y);
}

/* Updates sample event. */
void TrackBlock::updateSampleEvent(const SDL_Event * event) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (event->type == SDL_MOUSEBUTTONDOWN &&
        isInMouseArea(event->button.x, event->button.y)) {
        pressed = true;
    } else if (event->type == SDL_MOUSEBUTTONUP &&
               event->button.button == SDL_BUTTON_LEFT) {
        pressed = false;
    } else inMouseArea = isInMouseArea(x, y);
}

/* Checks if left mouse goes OFF->ON */
bool TrackBlock::isClicked() {
    bool clicked = (pressed != previousPressState && pressed);
    previousPressState = pressed;
    return clicked;
}

/* Moves sample attached to cursor if clicked */
void TrackBlock::updateSamplePosition() {
    if (pressed && isClicked()){
        fixedPosX = area.x;
        fixedPosY = area.y;
    }
    if (pressed) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        area.x = x - area.w / 2;
        area.y = y - area.h / 2;
    } else if (!pressed) {
        area.x = fixedPosX;
        area.y = fixedPosY;
    }
}

/* Copies sample attributes to grid block */
void TrackBlock::applySampleToGrid(TrackBlock & gridBlock) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (gridBlock.isInMouseArea(x, y) && mouseReleased()){
        gridBlock.type = this->type;
        gridBlock.texture = this->texture;
    }
}

/* Checks if mouse left button goes ON->OFF */
bool TrackBlock::mouseReleased() {
    bool released = (pressed != previousPressState && !pressed);
    previousPressState = pressed;
    return released;
}

/* Checks if cursor if over track block */
bool TrackBlock::isInMouseArea(int x, int y) {
    return  x >= area.x &&
            x <= (area.x + area.w) &&
            y >= area.y &&
            y <= (area.y + area.h);
}

/* Loads block texture */
SDL_Texture * TrackBlock::loadTexture(const std::string & img_path, SDL_Renderer * renderer) {
    SDL_Surface* rawImage = nullptr;
    SDL_Texture* tex = nullptr;
    rawImage = IMG_Load(img_path.c_str());
    if (rawImage == nullptr)
        throw Error("Loading Image: %s.", img_path.c_str());
    tex = SDL_CreateTextureFromSurface(renderer, rawImage);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    if (tex == nullptr )
        throw Error("Creating texture: %s.", img_path.c_str());
    SDL_FreeSurface(rawImage);
    return tex;
}

/* Gets block type */
trackPartType TrackBlock::getType() {
    return type;
}

TrackBlock::TrackBlock() = default;

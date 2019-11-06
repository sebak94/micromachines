//
// Created by fperezboco on 4/11/19.
//

#ifndef MAP_TRACKBLOCK_H
#define MAP_TRACKBLOCK_H


#include <SDL2/SDL_system.h>
#include <string>
#include <SDL2/SDL_events.h>
#include "Track.h"

#define SHADOW_FACTOR 150

class TrackBlock {
private:
    SDL_Rect area{};
    bool pressed = false;
    bool previousPressState = false;
    bool inMouseArea = false;
    SDL_Texture * texture{};
    trackPartType type;
    int fixedPosX = 0;
    int fixedPosY = 0;

public:
    TrackBlock();
    TrackBlock(SDL_Renderer *renderer, SDL_Rect &area, const std::string &path);
    void draw(SDL_Renderer *renderer);
    void updateEvent(const SDL_Event *event);
    SDL_Texture *loadTexture(const std::string &img_path, SDL_Renderer *renderer);
    void setBlock(SDL_Texture *tex, trackPartType t, int x, int y, int w, int h);
    trackPartType getType();
    bool isInMouseArea(int x, int y);

    void updateSampleEvent(const SDL_Event *event);

    void updateSamplePosition();

    void applySampleToGrid(TrackBlock &gridBlock);

    bool mouseReleased();

    bool isClicked();
};


#endif //MAP_TRACKBLOCK_H

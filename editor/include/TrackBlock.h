//
// Created by fperezboco on 4/11/19.
//

#ifndef MAP_TRACKBLOCK_H
#define MAP_TRACKBLOCK_H

#include <SDL2/SDL_system.h>
#include <string>
#include <SDL2/SDL_events.h>

#define SHADOW_FACTOR 150

#ifndef MAP_TRACKTYPE
#define MAP_TRACKTYPE
typedef enum {
    empty,
    downRight,
    downLeft,
    upRight,
    upLeft,
    horizontal,
    vertical,
    finishH,
    finishV,
    public1Up,
    public1Down,
    public1Left,
    public1Right
} trackPartType;
#endif

class TrackPartData;

class TrackBlock {
private:
    SDL_Rect area{};
    bool pressed = false;
    bool previousPressState = false;
    bool inMouseArea = false;
    bool rightClicked = false;
    SDL_Texture * texture{};
    SDL_Texture * startingTexture{};
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
    bool isRightClicked();
    void toEmpty();
};


#endif //MAP_TRACKBLOCK_H

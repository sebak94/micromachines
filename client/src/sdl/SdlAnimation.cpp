#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include "../../include/sdl/SdlAnimation.h"
#include "../../include/sdl/SdlSurface.h"

SdlAnimation::SdlAnimation(SdlTexture &texture, int framesInX,
                           int framesInY, int widthFrame, int heightFrame,
                           int spriteLen) : texture(texture) {
    this->spriteLen = spriteLen;
    int xValue = 0;
    int yValue = 0;
    //Creo un vector con los frames para la animacion
    for (int y = 0; y < framesInY; y++) {
        for (int x = 0; x < framesInX; x++) {
            SDL_Rect rect = {xValue, yValue, widthFrame, heightFrame};
            this->spriteClips.push_back(rect);
            xValue += widthFrame;
        }
        xValue = 0;
        yValue += heightFrame;
    }
}

SdlAnimation::~SdlAnimation() {}

void SdlAnimation::trigger() {
    triggered = true;
}

bool SdlAnimation::isTriggered() {
    return triggered;
}

void SdlAnimation::render(SDL_Rect &sdlDest, SdlWindow &window) {
        if (!started) {
            started = true;
            spriteStart = SDL_GetTicks();
        }
        int now = SDL_GetTicks();
        if (now - spriteStart > spriteLen) {
            spriteStart = SDL_GetTicks();
            sprite++;
            if (sprite == spriteClips.size()) {
                sprite = 0;
                triggered = false;
                started = false;
            }
        } else {
            SDL_Rect currentClip = spriteClips[sprite];
            this->texture.render(currentClip, sdlDest);
        }
}

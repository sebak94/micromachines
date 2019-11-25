#ifndef __SDLANIMATION_H__
#define __SDLANIMATION_H__

#include <vector>
#include "SdlWindow.h"
#include "SdlTexture.h"

class SdlAnimation {
private:
    size_t sprite = 0;
    SdlTexture &texture;
    std::vector<SDL_Rect> spriteClips;
    std::vector<SDL_Rect>::iterator spIt;
    int spriteStart;
    int spriteLen;  // ms
    bool started = false;
    bool triggered = false;

public:
    SdlAnimation(SdlTexture &texture, int framesInX,
                 int framesInY, int widthFrame, int heightFrame,
                 int spriteLen);
    ~SdlAnimation();
    void renderLooped(SDL_Rect &sdlDest, SdlWindow &window);
    void render(SDL_Rect &sdlDest, SdlWindow &window);
    bool isTriggered();
    void trigger();
    void render(SdlWindow &window);
};

#endif

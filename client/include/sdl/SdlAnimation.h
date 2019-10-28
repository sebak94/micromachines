#ifndef __SDLANIMATION_H__
#define __SDLANIMATION_H__

#include <vector>
#include "SdlWindow.h"
#include "SdlTexture.h"

class SdlAnimation {
private:
    int frames;
    SdlTexture &texture;
    std::vector<SDL_Rect> spriteClips;

public:
    SdlAnimation(SdlTexture &texture, int framesInX, int framesInY, int widthFrame, int heightFrame);
    ~SdlAnimation();
    void render(SDL_Rect &sdlDest, SdlWindow &window);
};

#endif

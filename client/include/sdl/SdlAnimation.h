#ifndef __SDLANIMATION_H__
#define __SDLANIMATION_H__

#include <vector>
#include "SdlWindow.h"
#include "SdlTexture.h"

class SdlAnimation {
private:
    int frames;
    SdlWindow &window;
    SdlTexture texture;
    std::vector<SDL_Rect> spriteClips;

public:
    SdlAnimation(int framesInX, int framesInY, const char* filename, SdlWindow &window, int widthFrame, int heightFrame);
    ~SdlAnimation();
    void render(SDL_Rect sdlDest);
};

#endif

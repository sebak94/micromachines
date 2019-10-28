#ifndef __SDLTEXTURE_H__
#define __SDLTEXTURE_H__

#include <string>
#include "SdlWindow.h"

class SdlTexture {
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public:
    SdlTexture(const std::string &filename, const SdlWindow& window);
    ~SdlTexture();
    int render(SDL_Rect &sdlSrc, SDL_Rect &sdlDest) const;

private:
    SDL_Texture* loadTexture(const std::string &filename);
};

#endif

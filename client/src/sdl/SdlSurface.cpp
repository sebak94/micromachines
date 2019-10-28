#include <SDL2/SDL_image.h>
#include <string>
#include "../../include/sdl/SdlSurface.h"
#include "../../include/sdl/SdlException.h"

SdlSurface::SdlSurface(const std::string &filename) {
    this->surface = loadSurface(filename);
}

SdlSurface::~SdlSurface() {
    SDL_FreeSurface(this->surface);
}

SDL_Surface* SdlSurface::loadSurface(const std::string &filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        throw SdlException("Error al cargar la superficie", SDL_GetError());
    }
    return surface;
}

int SdlSurface::render(SDL_Rect& sdlDest, const SdlWindow& window) const {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window.getRenderer(), this->surface);
    return SDL_RenderCopy(window.getRenderer(), texture, NULL, &sdlDest);
}

int SdlSurface::renderRotate(SDL_Rect& sdlDest, double angle, SDL_RendererFlip flip, const SdlWindow& window) const {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window.getRenderer(), this->surface);
    return SDL_RenderCopyEx(window.getRenderer(), texture, NULL, &sdlDest, angle, NULL, flip);
}

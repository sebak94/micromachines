#include "../../include/sdl/SdlTexture.h"
#include "../../include/sdl/SdlException.h"
#include <SDL2/SDL_image.h>
#include <string>

SdlTexture::SdlTexture(const std::string &filename, const SdlWindow& window)
        : renderer(window.getRenderer()) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        throw SdlException("Error en la inicialización de la imagen", IMG_GetError());
    }
    this->texture = loadTexture(filename);
}

SdlTexture::~SdlTexture() {
    SDL_DestroyTexture(this->texture);
    IMG_Quit();
}

SDL_Texture* SdlTexture::loadTexture(const std::string &filename) {
    SDL_Texture* texture = IMG_LoadTexture(this->renderer,
                                           filename.c_str());
    if (!texture) {
        throw SdlException("Error al cargar la textura", SDL_GetError());
    }
    return texture;
}

int SdlTexture::render(SDL_Rect sdlSrc, SDL_Rect sdlDest) const {
    return SDL_RenderCopy(this->renderer, this->texture, &sdlSrc, &sdlDest);
}

#include <SDL2/SDL.h>
#include "../../include/sdl/SdlWindow.h"
#include "../../include/sdl/SdlException.h"

SdlWindow::SdlWindow(int width, int height) :
        width(width), height(height) {
    int errCode = SDL_Init(SDL_INIT_VIDEO);
    if (errCode) {
        throw SdlException("Error en la inicialización", SDL_GetError());
    }
    errCode = SDL_CreateWindowAndRenderer(
            width, height, SDL_RENDERER_ACCELERATED,
            &this->window, &this->renderer);
    if (errCode) {
        throw SdlException("Error al crear ventana", SDL_GetError());
    }
}

SdlWindow::~SdlWindow() {
    if (this->renderer) {
        SDL_DestroyRenderer(this->renderer);
        this->renderer = nullptr;
    }
    if (this->window) {
        SDL_DestroyWindow(this->window);
        this->window = nullptr;
    }
    SDL_Quit();
}

int SdlWindow::getWidth() const {
    return this->width;
}

int SdlWindow::getHeight() const {
    return this->height;
}

void SdlWindow::fill() {
    SDL_SetRenderDrawColor(this->renderer, 0x33,0x33,0x33,0xFF);
    SDL_RenderClear(this->renderer);
}

void SdlWindow::render() {
    SDL_RenderPresent(this->renderer);
}

SDL_Renderer* SdlWindow::getRenderer() const {
    return this->renderer;
}

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include "../../include/sdl/SdlAnimation.h"
#include "../../include/sdl/SdlSurface.h"

SdlAnimation::SdlAnimation(SdlTexture &texture, int framesInX, int framesInY, int widthFrame, int heightFrame) : texture(texture) {
    this->frames = framesInX * framesInY;
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

void SdlAnimation::render(SDL_Rect sdlDest, SdlWindow &window) {
    int startTime = SDL_GetTicks();
    int animationRate = this->frames;
    int animationLength = this->frames + 1; //cantidad de frames + 1
    int milliseconds = 1000;
    int frameToDraw = 0;

    while (frameToDraw < animationRate) {
        frameToDraw = ((SDL_GetTicks() - startTime) * animationRate / milliseconds) % animationLength;
        SDL_Rect currentClip = this->spriteClips[frameToDraw];
        //Lleno la pantalla para "borrar" lo anterior
        window.fill(); //window.fill() tiene que cargar todo el fondo
        this->texture.render(currentClip, sdlDest);
        //Update screen
        window.render();
    }
}

//
// Created by fperezboco on 3/11/19.
//

#ifndef MAP_TEXTTEXTURE_H
#define MAP_TEXTTEXTURE_H


#include <SDL2/SDL_system.h>

class TextTexture {
private:
    SDL_Texture* textTexture;
    SDL_Rect fieldBox;
    SDL_Rect fieldBoxBorder;
    SDL_Color fieldColor;
    int width;
    int height;

public:
    void clearText();
    void textToTexture(SDL_Renderer *renderer, const std::string &textureText, SDL_Color textColor,
                       const char *fontName, int fontSize);
    void render(SDL_Renderer *renderer,
                int posX,
                int posY,
                SDL_Rect *rect = nullptr,
                SDL_Point *center = nullptr,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
    int getWidth();
    int getHeight();
    void createFieldBox(int posX, int posY, SDL_Color color);
    void renderFieldBox(SDL_Renderer *renderer);
};


#endif //MAP_TEXTTEXTURE_H

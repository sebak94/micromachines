//
// Created by fperezboco on 3/11/19.
//

#include <SDL2/SDL_ttf.h>
#include <string>
#include "../include/TextTexture.h"
#include "../../editor/include/Error.h"
#include "../../editor/include/Window.h"
#define BOX_BORDER_THICKNESS 3

/* Transforms string to texture with specific color, font and fontsize */
void TextTexture::textToTexture(SDL_Renderer *renderer,
                                const std::string &textureText,
                                SDL_Color textColor,
                                const char *fontName,
                                int fontSize) {
    TTF_Font *font = TTF_OpenFont(fontName, fontSize);
    clearText();
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if( textSurface != nullptr )
    {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface );
        if(textTexture == nullptr )
            throw Error( "Creating text texture %s", SDL_GetError());
        else
        {
            width = textSurface->w;
            height = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    } else {
        throw Error("Rendering text. SDL TTF code: %s", TTF_GetError());
    }
    TTF_CloseFont(font);
}

/* Destroys text texture */
void TextTexture::clearText() {
    if(textTexture != nullptr)
    {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
        width = 0;
        height = 0;
    }
}

/* Renders text texture in specific position being clipped by rect */
void TextTexture::render(SDL_Renderer *renderer,
                         int posX,
                         int posY,
                         SDL_Rect* rect,
                         SDL_Point* center,
                         SDL_RendererFlip flip) {
    SDL_Rect textBoxDimensions = {posX, posY, width, height};
    if( rect != nullptr )
    {
        textBoxDimensions.w = rect->w;
        textBoxDimensions.h = rect->h;
    }
    SDL_RenderCopyEx(renderer, textTexture, rect, &textBoxDimensions, 0, center, flip);
}

/* Gets texture width in pixels */
int TextTexture::getWidth() {
    return width;
}

/* Gets texture height in pixels */
int TextTexture::getHeight() {
    return height;
}

/* Creates box with border for user to write in */
void TextTexture::createFieldBox(int posX, int posY, SDL_Color color) {
    int boxBorderFactor = BOX_BORDER_THICKNESS;
    fieldColor = color;
    fieldBox = {fieldBox.x = posX,
                fieldBox.y = posY,
                fieldBox.w = (WINDOW_W - 2*posX),
                fieldBox.h = height};
    fieldBoxBorder = {fieldBoxBorder.x = posX-boxBorderFactor,
            fieldBoxBorder.y = posY-boxBorderFactor,
            fieldBoxBorder.w = fieldBox.w + 2*boxBorderFactor,
            fieldBoxBorder.h = fieldBox.h + 2*boxBorderFactor};
}

/* Renders box where the user writes */
void TextTexture::renderFieldBox(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer,
                           fieldColor.r-25,
                           fieldColor.g-25,
                           fieldColor.b-25,
                           0);
    SDL_RenderFillRect(renderer, &fieldBoxBorder);
    SDL_SetRenderDrawColor(renderer,
                           fieldColor.r,
                           fieldColor.g,
                           fieldColor.b,
                           0);
    SDL_RenderFillRect(renderer, &fieldBox);
}
//
// Created by fperezboco on 20/10/19.
//

#ifndef TP_BUTTON_H
#define TP_BUTTON_H

#include <string>
#include <cstdint>
#include <SDL2/SDL_events.h>

class Button {
private:
    SDL_Rect area{};
    bool pressed = false;
    bool previousPressState = false;
    SDL_Texture * texture{};

public:
    Button();
    //~Button();
    Button(SDL_Renderer *renderer, SDL_Rect &area, const std::string &path);
    void draw(SDL_Renderer *renderer);
    void updateEvent(const SDL_Event *event);
    bool isClicked();
    static SDL_Texture *loadTexture(const std::string &img_path, SDL_Renderer *renderer);

    void drawRotated(SDL_Renderer *renderer, double angle);

    void updatePos(int x, int y);
    void updateSize(int w, int h);

    void clean();
};


#endif //TP_BUTTON_H

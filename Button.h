//
// Created by fperezboco on 20/10/19.
//

#ifndef TP_BUTTON_H
#define TP_BUTTON_H


#include <cstdint>

class Button {
private:
    uint8_t  r, g, b, a;  // red, green, blue, alpha
    SDL_Rect area;
    bool pressed = false;
    bool previousPressState = false;


public:
    Button(uint8_t r, uint8_t g, uint8_t b, uint8_t a, SDL_Rect &area);
    void draw(SDL_Renderer *renderer);
    void updateEvent(const SDL_Event *event);

    bool isClicked();
};


#endif //TP_BUTTON_H

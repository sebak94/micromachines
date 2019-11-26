#ifndef __COLOR_H__
#define __COLOR_H__

#include <string>

enum ColorType : int { blue, white, black, yellow, red };

class Color {
    private:
    ColorType color;

    public:
    explicit Color(ColorType color);
    std::string name();
};

#endif

#ifndef __COLOR_H__
#define __COLOR_H__

#include <string>

enum ColorType { blue, white, black, yellow };

class Color {
    private:
    ColorType color;

    public:
    Color(ColorType color);
    std::string name();
};

#endif

#include "../../../include/model/cars/color.h"
#include <string>

Color::Color(ColorType color): color(color) {}

std::string Color::name() {
    switch (color) {
        case blue: return "blue";
        case white: return "white";
        case black: return "black";
        case yellow: return "yellow";
        default: return "";
    }
}

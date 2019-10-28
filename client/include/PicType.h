#ifndef __ROADTYPE_H__
#define __ROADTYPE_H__

#include <string>

enum class PicType : char {
    //Para la pista
    ROAD_BACK = 13,
    GRASS_BACK = 14,

    LINE_VERTICAL_UP = 0,
    LINE_VERTICAL_DOWN = 1,
    LINE_HORIZONTAL_LEFT = 2,
    LINE_HORIZONTAL_RIGHT = 3,

    CURVE_UP_TO_RIGHT = 4,
    CURVE_DOWN_TO_LEFT = 5,
    CURVE_LEFT_TO_UP = 6,
    CURVE_RIGHT_TO_DOWN = 7,

    CURVE_DOWN_TO_RIGHT = 8,
    CURVE_UP_TO_LEFT = 9,
    CURVE_RIGHT_TO_UP = 10,
    CURVE_LEFT_TO_DOWN = 11,

    //Para los autos
    CAR_RED = 12,
    CAR_BLACK = 15,
    CAR_BLUE = 16,
    CAR_GREEN = 17,
    CAR_ORANGE = 18,
    CAR_WHITE = 19,
    CAR_YELLOW = 20
};

#endif

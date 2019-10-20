#ifndef __TRACK_H__
#define __TRACK_H__

#include "model.h"
#include "point.h"
#include <vector>
#include <cstdint>

class Track {
    private:
    std::vector<Point> trajectory;
    uint8_t width;

    public:
    Track();
    ~Track();
};

#endif

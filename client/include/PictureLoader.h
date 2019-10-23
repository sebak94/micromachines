#ifndef __PICTURELOADER_H__
#define __PICTURELOADER_H__

#include <map>
#include "sdl/SdlSurface.h"

class PictureLoader {
private:
    std::map<std::string, SdlSurface*> &pictures;

public:
    PictureLoader(std::map<std::string, SdlSurface*> &pictures);
    ~PictureLoader();
};

#endif

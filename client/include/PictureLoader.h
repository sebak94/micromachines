#ifndef __PICTURELOADER_H__
#define __PICTURELOADER_H__

#include <map>
#include "sdl/SdlSurface.h"
#include "PicType.h"

class PictureLoader {
private:
    std::map<PicType, SdlSurface*> &pictures;

public:
    PictureLoader(std::map<PicType, SdlSurface*> &pictures);
    ~PictureLoader();
};

#endif

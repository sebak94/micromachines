#ifndef __PICTURELOADER_H__
#define __PICTURELOADER_H__

#include <map>
#include "sdl/SdlSurface.h"
#include "../../common/include/TrackPartData.h"
#include "../../common/include/Track.h"

class PictureLoader {
private:
    SdlWindow &window;
    std::map<std::string, SdlSurface*> &pictures;
    std::map<trackPartType, SdlSurface*> &trackPictures;

public:
    PictureLoader(SdlWindow &window, std::map<std::string, SdlSurface*> &pictures,
                    std::map<trackPartType, SdlSurface*> &trackPictures);
    ~PictureLoader();
};

#endif

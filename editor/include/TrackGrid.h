//
// Created by fperezboco on 4/11/19.
//

#ifndef MAP_TRACKGRID_H
#define MAP_TRACKGRID_H

#include <vector>
#include <map>
#include "TrackBlock.h"
#include "Window.h"
#include "../../common/include/Track.h"
#include "../../common/include/Button.h"

#define IMG_GRASS_PATH "../assets/images/grass.png"
#define IMG_DOWNRIGHT_PATH "../assets/images/downRight.png"
#define IMG_DOWNLEFT_PATH "../assets/images/downLeft.png"
#define IMG_UPRIGHT_PATH "../assets/images/upRight.png"
#define IMG_UPLEFT_PATH "../assets/images/upLeft.png"
#define IMG_HORIZONTAL_PATH "../assets/images/horizontal.png"
#define IMG_VERTICAL_PATH "../assets/images/vertical.png"
#define IMG_PUB1UP "../assets/images/public1Up.png"
#define IMG_PUB1DOWN "../assets/images/public1Down.png"
#define IMG_PUB1RIGHT "../assets/images/public1Right.png"
#define IMG_PUB1LEFT "../assets/images/public1Left.png"
#define IMG_FINISH_HORIZ "../assets/images/finishHorizontal.png"
#define IMG_FINISH_VERT "../assets/images/finishVertical.png"

#define TEX_GRASS "grass"
#define TEX_DOWNRIGHT "downRight"
#define TEX_DOWNLEFT "downLeft"
#define TEX_UPRIGHT "upRight"
#define TEX_UPLEFT "upLeft"
#define TEX_HORIZONTAL "horizontal"
#define TEX_VERTICAL "vertical"
#define TEX_FINISH_V "finishVert"
#define TEX_FINISH_H "finishHoriz"
#define TEX_PUB1UP "public1Up"
#define TEX_PUB1DOWN "public1Down"
#define TEX_PUB1RIGHT "public1Right"
#define TEX_PUB1LEFT "public1Left"

#define GRID_MARGIN_SIZE 40
#define SAMPLES_MARGIN 150
#define NUMBER_OF_SAMPLES 6
#define SAMPLE_SEPARATION_FACTOR 15

class TrackGrid {
private:
    std::vector<TrackBlock> grid;
    std::vector<TrackBlock> samples;
    std::map<std::string, SDL_Texture*> textures;
    int wBlocks = 0;
    int hBlocks = 0;
    int gridMarginWidth = 0;
    int gridMarginHeight = 0;
    int gridSize = 0;
    int blockWidth = 0;
    int blockHeight = 0;

public:
    TrackGrid();
    TrackGrid(Window &game, int widthBlocks, int heightBlocks);
    static SDL_Texture *loadTexture(const std::string &img_path, SDL_Renderer *renderer);
    void loadTextures(SDL_Renderer *renderer);
    trackPartType getType(int i);
    int getSize();
    void draw(SDL_Renderer *renderer);
    void initGrid();
    void setGridSize();
    void updateTrackBlockEvent(const SDL_Event *event, int index);
    void createSamples();
    int getGridMarginHeight();
    void updateSamples(const SDL_Event *event);
    void applyAllSamplesToGrid();
    void applySampleToGrid(TrackBlock &sample);
    bool findStartLine(int &startRow, int &startCol);
    int getPixelPosX(int index);

    int getPixelPosY(int row);

    trackPartType getType(int row, int col);

    TrackGrid(Window &game, Track loadedTrack);

    void initGridFromTrack(const Track &loadedTrack);

    SDL_Texture *getTexture(trackPartType type);
};


#endif //MAP_TRACKGRID_H

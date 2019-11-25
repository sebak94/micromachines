//
// Created by fperezboco on 4/11/19.
//

#include <SDL2/SDL_image.h>

#include "../include/TrackGrid.h"
#include "../include/Window.h"

/* Creates track grid of trackBlocks for editor */
TrackGrid::TrackGrid(Window & game, int widthBlocks, int heightBlocks) {
    wBlocks = widthBlocks;
    hBlocks = heightBlocks;
    setGridSize();
    loadTextures(game.renderer);
    initGrid();
}

/* Creates track grid of trackBlocks for editor from existent track */
TrackGrid::TrackGrid(Window & game, Track loadedTrack) {
    wBlocks = loadedTrack.getTrackW();
    hBlocks = loadedTrack.getTrackH();
    setGridSize();
    loadTextures(game.renderer);
    initGridFromTrack(loadedTrack);
}

/* Configs grid size in pixels based on widest dimension.
 * Also configs blocks size and adjust them to window size.
 * Considers non-square tracks.*/
void TrackGrid::setGridSize() {
    if (wBlocks >= hBlocks) {
        gridMarginWidth = GRID_MARGIN_SIZE + SAMPLES_MARGIN;
        gridSize = WINDOW_W - 2 * gridMarginWidth;
        blockWidth = gridSize / wBlocks;
        blockHeight = gridSize / wBlocks;
        gridMarginHeight = WINDOW_H / 2 - blockHeight*hBlocks/2;
    } else {
        gridMarginHeight = GRID_MARGIN_SIZE;
        gridSize = WINDOW_H - 2 * gridMarginHeight;
        blockWidth = gridSize / hBlocks;
        blockHeight = gridSize / hBlocks;
        gridMarginWidth = WINDOW_W / 2 - blockWidth*wBlocks/2;
    }
}

/* Creates empty grid */
void TrackGrid::initGrid() {
    TrackBlock auxBlock;
    int row = 0;
    int col = 0;
    for (int i = 1; i <= wBlocks*hBlocks; i++) {
        col = (i-1)%wBlocks, row = (i-1)/wBlocks;
        auxBlock.setBlock(textures[TEX_GRASS],
                empty,
                col * blockWidth + gridMarginWidth,
                row * blockHeight + gridMarginHeight,
                blockWidth, blockHeight);
        grid.emplace_back(auxBlock);
    }
}

/* Creates empty grid */
void TrackGrid::initGridFromTrack(const Track & loadedTrack) {
    TrackBlock auxBlock;
    TrackPartData auxPart;
    std::vector<TrackPartData> parts = loadedTrack.getTrackPartData();
    int row = 0;
    int col = 0;
    for (int i = 1; i <= wBlocks*hBlocks; i++) {
        col = (i-1)%wBlocks, row = (i-1)/wBlocks;
        auxPart = parts[row*wBlocks + col];
        auxBlock.setBlock(getTexture(auxPart.getType()),
                          auxPart.getType(),
                          col * blockWidth + gridMarginWidth,
                          row * blockHeight + gridMarginHeight,
                          blockWidth, blockHeight);
        grid.emplace_back(auxBlock);
    }
}

/* Loads one texture with blending options */
SDL_Texture * TrackGrid::loadTexture(const std::string & img_path, SDL_Renderer * renderer) {
    SDL_Surface* rawImage = nullptr;
    SDL_Texture* tex = nullptr;
    rawImage = IMG_Load(img_path.c_str());
    if (rawImage == nullptr)
        throw Error("Loading Image: %s.", img_path.c_str());
    tex = SDL_CreateTextureFromSurface(renderer, rawImage);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    if (tex == nullptr)
        throw Error("Creating texture: %s.", img_path.c_str());
    SDL_FreeSurface(rawImage);
    return tex;
}

/* Gets trackBlock type */
trackPartType TrackGrid::getType(int i) {
    return grid[i].getType();
}

trackPartType TrackGrid::getType(int row, int col) {
    return grid[row * wBlocks + col].getType();
}

/* Gets grid size width*height */
int TrackGrid::getSize() {
    return grid.size();
}

/* Draws samples and grid */
void TrackGrid::draw(SDL_Renderer * renderer) {
    for (auto & trackBlock : grid)
        trackBlock.draw(renderer);
    for (auto & trackSample : samples)
        trackSample.draw(renderer);
}

/* Updates one block in grid */
void TrackGrid::updateTrackBlockEvent(const SDL_Event *event, int index) {
    grid[index].updateEvent(event);
}

/* Updates all samples events and positions */
void TrackGrid::updateSamples(const SDL_Event *event) {
    for (auto & trackSample : samples) {
        trackSample.updateSampleEvent(event);
        trackSample.updateSamplePosition();
    }
}

/* Gets finish line position. Returns false if non-existent or duplicated */
bool TrackGrid::findStartLine(int & startRow, int & startCol) {
    trackPartType type;
    bool found = false;
    for (int i = 0; i < grid.size(); i++) {
        type = grid[i].getType();
        if ((type == finishH || type == finishV) && found)
            return false;  // avoids double finish lines;
        if ((type == finishH || type == finishV)) {
            found = true;
            startCol = i % wBlocks;
            startRow = i / wBlocks;
        }
    }
    if (found)
        return true;
}

int TrackGrid::getPixelPosX(int col) {
    return col*blockWidth + blockWidth/2 + gridMarginWidth;
}

int TrackGrid::getPixelPosY(int row) {
    return row*blockHeight + blockHeight/2 + gridMarginHeight;
}

/* Loads all textures */
void TrackGrid::loadTextures(SDL_Renderer * renderer){
    textures.emplace(TEX_GRASS, loadTexture(IMG_GRASS_PATH, renderer));
    textures.emplace(TEX_HORIZONTAL, loadTexture(IMG_HORIZONTAL_PATH, renderer));
    textures.emplace(TEX_VERTICAL, loadTexture(IMG_VERTICAL_PATH, renderer));
    textures.emplace(TEX_DOWNLEFT, loadTexture(IMG_DOWNLEFT_PATH, renderer));
    textures.emplace(TEX_DOWNRIGHT, loadTexture(IMG_DOWNRIGHT_PATH, renderer));
    textures.emplace(TEX_UPLEFT, loadTexture(IMG_UPLEFT_PATH, renderer));
    textures.emplace(TEX_UPRIGHT, loadTexture(IMG_UPRIGHT_PATH, renderer));
    textures.emplace(TEX_FINISH_H, loadTexture(IMG_FINISH_HORIZ, renderer));
    textures.emplace(TEX_FINISH_V, loadTexture(IMG_FINISH_VERT, renderer));
    textures.emplace(TEX_PUB1DOWN, loadTexture(IMG_PUB1DOWN, renderer));
    textures.emplace(TEX_PUB1LEFT, loadTexture(IMG_PUB1LEFT, renderer));
    textures.emplace(TEX_PUB1RIGHT, loadTexture(IMG_PUB1RIGHT, renderer));
    textures.emplace(TEX_PUB1UP, loadTexture(IMG_PUB1UP, renderer));
}

/* Gets textures */
SDL_Texture * TrackGrid::getTexture(trackPartType type){
    switch(type){
        case empty: return textures[TEX_GRASS];
        case downRight: return textures[TEX_DOWNRIGHT];
        case downLeft: return textures[TEX_DOWNLEFT];
        case upRight: return textures[TEX_UPRIGHT];
        case upLeft: return textures[TEX_UPLEFT];
        case horizontal: return textures[TEX_HORIZONTAL];
        case vertical: return textures[TEX_VERTICAL];
        case finishH: return textures[TEX_FINISH_H];
        case finishV: return textures[TEX_FINISH_V];
        case public1Up: return textures[TEX_PUB1UP];
        case public1Down: return textures[TEX_PUB1DOWN];
        case public1Left: return textures[TEX_PUB1LEFT];
        case public1Right: return textures[TEX_PUB1RIGHT];
    }
}

/* Loads and configs samples considering window and grid size */
void TrackGrid::createSamples() {
    int sep = SAMPLE_SEPARATION_FACTOR;  // between samples
    int samplesHeight = WINDOW_H - 2 * GRID_MARGIN_SIZE;  // height of grid
    int s = (samplesHeight + sep) / NUMBER_OF_SAMPLES - sep;// // sample size
    int y = GRID_MARGIN_SIZE;  // sample X position
    int x = (gridMarginWidth - s)/2;  // sample Y position
    TrackBlock auxBlock;

    auxBlock.setBlock(textures[TEX_HORIZONTAL], horizontal, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_VERTICAL], vertical, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_DOWNLEFT], downLeft, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_DOWNRIGHT], downRight, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_UPLEFT], upLeft, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_UPRIGHT], upRight, x, y, s, s);
    samples.emplace_back(auxBlock);

    y = GRID_MARGIN_SIZE;
    x = WINDOW_W - (gridMarginWidth + s)/2;
    auxBlock.setBlock(textures[TEX_FINISH_V], finishV, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_FINISH_H], finishH, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_PUB1UP], public1Up, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_PUB1RIGHT], public1Right, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_PUB1DOWN], public1Down, x, y, s, s);
    y += s + sep; samples.emplace_back(auxBlock);
    auxBlock.setBlock(textures[TEX_PUB1LEFT], public1Left, x, y, s, s);
    samples.emplace_back(auxBlock);
}

/* Copies sample attributes to 1 block in grid if cursor position match*/
void TrackGrid::applySampleToGrid(TrackBlock & sample) {
    for (auto & trackBlock : grid){
        if (trackBlock.isRightClicked())
            trackBlock.toEmpty();
        sample.applySampleToGrid(trackBlock);

    }
}

/* Checks all samples with grid position */
void TrackGrid::applyAllSamplesToGrid() {
    for (auto & trackSample : samples)
        applySampleToGrid(trackSample);
}

/* Returns size of top/bottom margin */
int TrackGrid::getGridMarginHeight() {
    return gridMarginHeight;
}

TrackGrid::TrackGrid() = default;

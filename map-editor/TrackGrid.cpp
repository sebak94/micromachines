//
// Created by fperezboco on 4/11/19.
//

#include <SDL2/SDL_image.h>

#include "TrackGrid.h"
#include "Window.h"

/* Creates track grid of trackBlocks for editor */
TrackGrid::TrackGrid(Window & game, int widthBlocks, int heightBlocks) {
    wBlocks = widthBlocks;
    hBlocks = heightBlocks;
    setGridSize();
    loadTextures(game.renderer);
    initGrid();
}

/* Configs grid size in pixels based on widest dimension.
 * Also configs blocks size and adjust them to window size.
 * Considers non-square tracks.*/
void TrackGrid::setGridSize() {
    if (wBlocks >= hBlocks) {
        gridMarginWidth = WINDOW_W / GRID_MARGIN_FACTOR;
        gridSize = WINDOW_W - 2 * gridMarginWidth;
        blockWidth = gridSize / wBlocks;
        blockHeight = gridSize / wBlocks;
        gridMarginHeight = WINDOW_H / 2 - blockHeight*hBlocks/2;
    } else {
        gridMarginHeight = WINDOW_H / GRID_MARGIN_FACTOR;
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
        if (col == wBlocks){
            col = 0;
            row++;
        }
        auxBlock.setBlock(textures[TEX_GRASS],
                empty,
                col * blockWidth + gridMarginWidth,
                row * blockHeight + gridMarginHeight,
                blockWidth, blockHeight);
        grid.emplace_back(auxBlock);
        col++;
    }
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

/* Gets grid size width*height */
int TrackGrid::getSize() {
    return grid.size();
}

/* Draws samples and grid */
void TrackGrid::draw(SDL_Renderer * renderer) {
    for (auto & trackBlock : grid)
        trackBlock.draw(renderer);
    grassSample.draw(renderer);
    horizSample.draw(renderer);
    verticalSample.draw(renderer);
    downRightSample.draw(renderer);
    downLeftSample.draw(renderer);
    upRightSample.draw(renderer);
    upLeftSample.draw(renderer);
}

/* Updates one block in grid */
void TrackGrid::updateTrackBlockEvent(const SDL_Event *event, int index) {
    grid[index].updateEvent(event);
}

/* Updates all samples events and positions */
void TrackGrid::updateSamples(const SDL_Event *event) {
    grassSample.updateSampleEvent(event);
    grassSample.updateSamplePosition();
    horizSample.updateSampleEvent(event);
    horizSample.updateSamplePosition();
    verticalSample.updateSampleEvent(event);
    verticalSample.updateSamplePosition();
    downRightSample.updateSampleEvent(event);
    downRightSample.updateSamplePosition();
    downLeftSample.updateSampleEvent(event);
    downLeftSample.updateSamplePosition();
    upRightSample.updateSampleEvent(event);
    upRightSample.updateSamplePosition();
    upLeftSample.updateSampleEvent(event);
    upLeftSample.updateSamplePosition();
}

/* Loads and configs samples considering window and grid size */
void TrackGrid::createSamples() {
    int sep = WINDOW_W/SAMPLE_SEPARATION_FACTOR;  // between samples
    int s = WINDOW_W/SAMPLE_SEPARATION_FACTOR;  // sample size in pixels
    int x = (WINDOW_W - NUMBER_OF_SAMPLES * (s + sep) + sep) / 2;
    int y = WINDOW_H/SAMPLE_SEPARATION_FACTOR;  // sample Y position

    grassSample.setBlock(textures[TEX_GRASS], empty, x, y, s, s);
    x += s + sep;
    horizSample.setBlock(textures[TEX_HORIZONTAL], horizontal, x, y, s, s);
    x += s + sep;
    verticalSample.setBlock(textures[TEX_VERTICAL], vertical, x, y, s, s);
    x += s + sep;
    downLeftSample.setBlock(textures[TEX_DOWNLEFT], downLeft, x, y, s, s);
    x += s + sep;
    downRightSample.setBlock(textures[TEX_DOWNRIGHT], downRight, x, y, s, s);
    x += s + sep;
    upLeftSample.setBlock(textures[TEX_UPLEFT], upLeft, x, y, s, s);
    x += s + sep;
    upRightSample.setBlock(textures[TEX_UPRIGHT], upRight, x, y, s, s);
}

/* Copies sample attributes to 1 block in grid if cursor position match*/
void TrackGrid::applySampleToGrid(TrackBlock & sample) {
    for (auto & trackBlock : grid)
        sample.applySampleToGrid(trackBlock);
}

/* Checks all samples with grid position */
void TrackGrid::applyAllSamplesToGrid() {
    applySampleToGrid(grassSample);
    applySampleToGrid(horizSample);
    applySampleToGrid(verticalSample);
    applySampleToGrid(downRightSample);
    applySampleToGrid(downLeftSample);
    applySampleToGrid(upRightSample);
    applySampleToGrid(upLeftSample);
}

/* Returns size of top/bottom margin */

int TrackGrid::getGridMarginHeight() {
    return gridMarginHeight;
}

TrackGrid::TrackGrid() = default;

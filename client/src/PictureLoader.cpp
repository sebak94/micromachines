#include "../include/PictureLoader.h"

#define CAR_RED "red"
#define CAR_BLACK "black"
#define CAR_BLUE "blue"
#define CAR_GREEN "green"
#define CAR_ORANGE "orange"
#define CAR_WHITE "white"
#define CAR_YELLOW "yellow"
#define HEART "heart"
#define PODIUM "podium"
#define NUMBER_ONE "1"
#define NUMBER_TWO "2"
#define NUMBER_THREE "3"
#define NUMBER_FOUR "4"
#define NUMBER_FIVE "5"
#define MUD "mud"
#define OIL "oil"
#define STONES "stones"
#define HEALTHBOX "healthBox"
#define BOOST "boost"

PictureLoader::PictureLoader(SdlWindow &window, std::map<std::string, SdlSurface*> &pictures,
        std::map<trackPartType, SdlSurface*> &trackPictures) :
        window(window), pictures(pictures), trackPictures(trackPictures) {
    //Imagenes para la pista
    trackPictures[empty] = new SdlSurface("../assets/images/grass.png", window);
    trackPictures[downRight] = new SdlSurface("../assets/images/downRight.png", window);
    trackPictures[downLeft] = new SdlSurface("../assets/images/downLeft.png", window);
    trackPictures[upRight] = new SdlSurface("../assets/images/upRight.png", window);
    trackPictures[upLeft] = new SdlSurface("../assets/images/upLeft.png", window);
    trackPictures[horizontal] = new SdlSurface("../assets/images/horizontal.png", window);
    trackPictures[vertical] = new SdlSurface("../assets/images/vertical.png", window);
    trackPictures[finishH] = new SdlSurface("../assets/images/finishHorizontal.png", window);
    trackPictures[finishV] = new SdlSurface("../assets/images/finishVertical.png", window);
    trackPictures[public1Up] = new SdlSurface("../assets/images/public1Up.png", window);
    trackPictures[public1Right] = new SdlSurface("../assets/images/public1Right.png", window);
    trackPictures[public1Down] = new SdlSurface("../assets/images/public1Down.png", window);
    trackPictures[public1Left] = new SdlSurface("../assets/images/public1Left.png", window);

    //Imagenes de los autos
    pictures[CAR_RED] = new SdlSurface("../assets/images/car_red.png", window);
    pictures[CAR_BLACK] = new SdlSurface("../assets/images/car_black.png", window);
    pictures[CAR_BLUE] = new SdlSurface("../assets/images/car_blue.png", window);
    pictures[CAR_GREEN] = new SdlSurface("../assets/images/car_green.png", window);
    pictures[CAR_ORANGE] = new SdlSurface("../assets/images/car_orange.png", window);
    pictures[CAR_WHITE] = new SdlSurface("../assets/images/car_white.png", window);
    pictures[CAR_YELLOW] = new SdlSurface("../assets/images/car_yellow.png", window);

    //Imagenes varias
    pictures[HEART] = new SdlSurface("../assets/images/heart.png", window);
    pictures[PODIUM] = new SdlSurface("../assets/images/podium.png", window);
    pictures[NUMBER_ONE] = new SdlSurface("../assets/images/1.png", window);
    pictures[NUMBER_TWO] = new SdlSurface("../assets/images/2.png", window);
    pictures[NUMBER_THREE] = new SdlSurface("../assets/images/3.png", window);
    pictures[NUMBER_FOUR] = new SdlSurface("../assets/images/4.png", window);
    pictures[NUMBER_FIVE] = new SdlSurface("../assets/images/5.png", window);

    //Imagenes de modificadores
    pictures[MUD] = new SdlSurface("../assets/images/mud.png", window);
    pictures[OIL] = new SdlSurface("../assets/images/oil.png", window);
    pictures[STONES] = new SdlSurface("../assets/images/stones.png", window);
    pictures[HEALTHBOX] = new SdlSurface("../assets/images/healthBox.png", window);
    pictures[BOOST] = new SdlSurface("../assets/images/boost.png", window);
}

PictureLoader::~PictureLoader() {
    for (const auto pair : trackPictures) {
        delete pair.second;
    }
    for (const auto pair : pictures) {
        delete pair.second;
    }
}

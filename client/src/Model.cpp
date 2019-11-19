#include "../include/Model.h"
#include "../../common/include/TrackList.h"

Model::Model() {

}

Model::~Model() {
    for (const auto pair : cars) {
        delete pair.second;
    }
}

void Model::setTrack(std::vector<TrackPartData> track) {
    this->track = track;
}

void Model::setTotalLaps(const std::string& str) {
    size_t pos = str.find_last_of(',') + 1;
    int laps = stoi(parse(str, pos, '\n'));
    this->totalLaps = laps;
}

int Model::getTotalLaps() {
    return totalLaps;
}

std::vector<TrackPartData>& Model::getTrack() {
    return this->track;
}

void Model::setMyColor(std::string str) {
    size_t pos = str.find_last_of(',') + 1;
    std::string color = parse(str, pos, '\n');
    myColor = color;
}

void Model::setGameState(std::string str) {
    if (str == "G,mainMenu\n") gameState = mainMenu;
    else if (str == "G,creating\n") gameState = creating;
    else if (str == "G,joining\n") gameState = joining;
    else if (str == "G,waitingPlayers\n") gameState = waitingPlayers;
    else if (str == "G,startCountdown\n") gameState = startCountdown;
    else if (str == "G,playing\n") gameState = playing;
    else if (str == "G,waitingEnd\n") gameState = waitingEnd;
    else if (str == "G,gameEnded\n") gameState = gameEnded;
}

void Model::setGameState(GameState state) {
    gameState = state;
}

GameState Model::getGameState() {
    return gameState;
}

std::string Model::getMyColor() const {
    return myColor;
}

std::map<std::string, Car*>& Model::getCars() {
    return this->cars;
}

void Model::updateCar(std::string str) {
    //serializacion: current_velocity, health, rotation, x, y, color\n
    size_t pos = 0;
    int velocity = std::stoi(parse(str, pos, ','));
    int health = std::stoi(parse(str, pos, ','));
    int rotation = std::stoi(parse(str, pos, ','));
    int x = std::stoi(parse(str, pos, ','));
    int y = std::stoi(parse(str, pos, ','));
    int laps = std::stoi(parse(str, pos, ','));
    std::string color = parse(str, pos, '\n');

    if (cars[color] == nullptr) {
        cars[color] = new Car(color);
    }
    cars[color]->update(x, y, rotation, health, laps);
}

std::string Model::parse(const std::string &str, size_t &pos, const char delim) {
    std::string substr;
    size_t nextPos = str.find(delim, pos);
    size_t len = nextPos - pos;
    substr = str.substr(pos, len);
    pos = nextPos + 1;
    return substr;
}

void Model::setTrackNames(std::string tracks) {
    trackNames = TrackList::getTrackNames(tracks);
}

std::vector<std::string> Model::getTrackNames() {
    return trackNames;
}

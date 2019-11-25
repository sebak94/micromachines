#include "../include/Model.h"
#include "../../common/include/TrackList.h"
#include <algorithm>

Model::Model() {}

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

std::vector<TrackPartData> Model::getTrack() {
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

std::map<std::string, Car*> Model::getCars() {
    return this->cars;
}

void Model::updateCar(std::string str) {
    size_t pos = str.find_last_of(',') + 1;
    std::string color = parse(str, pos, '\n');
    if (cars[color] == nullptr) {
        cars[color] = new Car(str);
    }
    cars[color]->update(str);
}

void Model::createModifiers(std::string str) {
    ModifierList modifierList(str);
    modifiers = std::move(modifierList.getModifiers());
}

void Model::updateModifiers(std::string str) {
    ModifierList modifierList(str);
    modifiers = modifierList.getModifiers();
}

std::vector<Modifier> Model::getModifiers() {
    return this->modifiers;
}

bool Model::isBraking() {
    return braking;
}

void Model::setBrake() {
    braking = true;
}

void Model::unsetBrake() {
    braking = false;
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

void Model::setMatchNames(std::string matches) {
    if (matches.length() > 0) {
        matchNames = TrackList::getTrackNames(matches);
    }
}

std::vector<std::string> Model::getTrackNames() {
    return trackNames;
}

std::vector<std::string> Model::getMatchNames() {
    return matchNames;
}

std::vector<std::string> Model::getMatchResults() {
    return matchResults;
}

void Model::updateMatchResults(std::string results) {
    results.erase(results.length() -1); //borro el \n
    size_t pos = 0;
    while (pos < results.size()) {
        std::string car = parse(results, pos, ',');
        if (std::find(matchResults.begin(), matchResults.end(), car) == matchResults.end()) {
            matchResults.push_back(car);
        }
        if (pos == 0)
            break;
    }
}

void Model::reset() {
    matchNames.clear();
    matchResults.clear();
}

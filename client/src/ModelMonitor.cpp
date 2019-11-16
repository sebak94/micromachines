#include "../include/ModelMonitor.h"

ModelMonitor::ModelMonitor(Model &model) : model(model) {}

ModelMonitor::~ModelMonitor() {}

void ModelMonitor::setTrack(std::vector<TrackPartData> track) {
    std::lock_guard<std::mutex> lock(m);
    model.setTrack(track);
}

std::vector<TrackPartData>& ModelMonitor::getTrack() {
    std::lock_guard<std::mutex> lock(m);
    return model.getTrack();
}

void ModelMonitor::setMyColor(std::string str) {
    std::lock_guard<std::mutex> lock(m);
    model.setMyColor(str);
}

void ModelMonitor::setGameState(std::string str) {
    std::lock_guard<std::mutex> lock(m);
    model.setGameState(str);
}

void ModelMonitor::updateCar(std::string str) {
    std::lock_guard<std::mutex> lock(m);
    model.updateCar(str);
}

GameState ModelMonitor::getGameState() {
    std::lock_guard<std::mutex> lock(m);
    return model.getGameState();
}

std::string ModelMonitor::getMyColor() const {
    //es constante, no usa mutex
    return model.getMyColor();
}

std::map<std::string, Car*>& ModelMonitor::getCars() {
    std::lock_guard<std::mutex> lock(m);
    return model.getCars();
}

void ModelMonitor::setTotalLaps(std::string str) {
    //es constante, no usa mutex
    model.setTotalLaps(str);
}

int ModelMonitor::getTotalLaps() {
    std::lock_guard<std::mutex> lock(m);
    return model.getTotalLaps();
}

void ModelMonitor::setTrackNames(std::string tracks) {
    std::lock_guard<std::mutex> lock(m);
    model.setTrackNames(tracks);
}

std::vector<std::string> ModelMonitor::getTrackNames() {
    std::lock_guard<std::mutex> lock(m);
    return model.getTrackNames();
}

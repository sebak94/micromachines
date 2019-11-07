#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <map>
#include "Car.h"
#include "../../common/include/TrackPartData.h"

class Model {
private:
    std::map<std::string, Car*> cars;
    std::vector<TrackPartData> track;
    std::string myColor;

public:
    Model();
    ~Model();
    void setTrack(std::vector<TrackPartData> track);
    std::vector<TrackPartData>& getTrack();
    void setMyColor(std::string str); //Se usa una sola vez, para setear el color de mi auto
    std::string getMyColor() const;
    std::map<std::string, Car*>& getCars();
    void addCar(std::string str);
    void updateCar(std::string str);

private:
    std::string parse(const std::string &str, size_t &pos, const char delim);
};

#endif

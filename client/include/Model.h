#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <map>
#include "Car.h"
#include "../../common/include/TrackPartData.h"

class Model {
private:
    std::map<std::string, Car*> cars;
    std::vector<TrackPartData> trackPartData;
    std::string myColor;

public:
    Model();
    ~Model();
    std::map<std::string, Car*>& getCars();
    void setTrackPartData(std::vector<TrackPartData> trackPartData);
    std::vector<TrackPartData> getTrackPartData() const;
    void setMyColor(std::string str);
    std::string getMyColor() const;
    void addCar(std::string str);
    void updateCar(std::string str);

private:
    std::string parse(const std::string &str, size_t &pos, const char delim);
};

#endif

//
// Created by fperezboco on 22/11/19.
//

#ifndef MICROMACHINES_CONFIG_H
#define MICROMACHINES_CONFIG_H

#include <map>

class Config {
private:
    std::map<std::string, std::string> values;

public:
    Config();
    uint8_t getAsUint8(const std::string &request);
    std::string getAsString(const std::string &request);
    uint16_t getAsUint16(const std::string &request);
    int getAsInt(const std::string &request);
    bool isSet(const std::string &request);
    float getAsFloat(const std::string &request);
    double getAsDouble(const std::string &request);
};


#endif //MICROMACHINES_CONFIG_H

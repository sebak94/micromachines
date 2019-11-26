//
// Created by fperezboco on 22/11/19.
//

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include "../include/Config.h"
#include <iostream>

#define CONFIG_PATH "../config.json"
#define SET_CONDITION "true"

Config::Config() {
    Json::Value jsonValues;
    Json::Reader reader;
    std::ifstream configFile(CONFIG_PATH, std::ifstream::binary);
    if (!reader.parse(configFile, jsonValues)){
        std::cout << reader.getFormattedErrorMessages() << std::endl;
    } else {
        for (auto const & it : jsonValues.getMemberNames())
            values.emplace(std::string(it), jsonValues[it].asString());
    }
    configFile.close();
}

int Config::getAsInt(const std::string & request) {
    return stoi(values[request]);
}

bool Config::isSet(const std::string & request) {
    return values[request] == SET_CONDITION;
}

float Config::getAsFloat(const std::string & request) {
    return std::stof(values[request]);
}

double Config::getAsDouble(const std::string & request) {
    return std::stod(values[request]);
}

uint16_t Config::getAsUint16(const std::string & request) {
    return (uint16_t) stoi(values[request]);
}

std::string Config::getAsString(const std::string & request) {
    return values[request];
}
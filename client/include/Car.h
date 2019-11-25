#ifndef __CAR_H__
#define __CAR_H__

#include <string>

class Car {
private:
    int x;
    int y;
    int degrees;
    int health;
    int lastHealth = 100;
    bool collision = false;
    bool explosion = false;
    bool explosionSound = false;
    int laps = 0;
    int lastTrackPart = 0;
    std::string color;

public:
    explicit Car(std::string carStr);
    ~Car();
    void setCarParams(std::string carStr);
    int getX() const;
    int getY() const;
    int getDegrees() const;
    int getHealth() const;
    std::string getMyColor() const;
    void update(std::string carStr);
    int getMyLap() const;
    bool collided();
    bool exploded();
    bool explodedSound();

private:
    std::string parse(const std::string &str, size_t &pos, const char delim);
};

#endif

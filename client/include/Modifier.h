#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include <string>

typedef enum {
    healthBox,
    boost,
    stones,
    oil,
    mud
} ModType;

class Modifier {
private:
    int x;
    int y;
    ModType type;

public:
    Modifier();
    void setType(std::string str);
    explicit Modifier(const std::string &str);
    ~Modifier();
    int getX() const;
    int getY() const;
    std::string getType() const;

private:
    std::string parse(const std::string &str, size_t &pos, const char delim);
};

#endif

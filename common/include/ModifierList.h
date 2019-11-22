#ifndef __MODIFIERLIST_H__
#define __MODIFIERLIST_H__

#include <vector>
#include "Modifier.h"

#define MODIFIER_SIZE 25

class ModifierList {
private:
    std::vector<Modifier> modifiers;

    std::string parse(const std::string &str, size_t &pos, const char delim);

public:
    ModifierList();
    ModifierList(std::string str);
    ~ModifierList();
    std::string serialize();
    std::vector<Modifier>& getModifiers();
    bool isOnBoost(int posX, int posY);
};

#endif

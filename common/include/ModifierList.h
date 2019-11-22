#ifndef __MODIFIERLIST_H__
#define __MODIFIERLIST_H__

#include <vector>
#include "Modifier.h"

class ModifierList {
private:
    std::vector<Modifier> modifiers;

public:
    ModifierList();
    ModifierList(std::string str);
    ~ModifierList();
    std::string serialize();
    std::vector<Modifier>& getModifiers();

private:
    std::string parse(const std::string &str, size_t &pos, const char delim);
};

#endif

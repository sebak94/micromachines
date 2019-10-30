//
// Created by mati on 10/30/19.
//

#ifndef MICROMACHINES_DIRECTORY_PARSER_H
#define MICROMACHINES_DIRECTORY_PARSER_H

#include <sys/types.h>
#include <utility>
#include <iostream>
#include <vector>
#include <dirent.h>

class DirectoryParser {
private:
    DIR *dir;

public:
    DirectoryParser(const char *path);

    ~DirectoryParser();

    void read_directory(std::vector<std::string> &v);
};

#endif //MICROMACHINES_DIRECTORY_PARSER_H

//
// Created by mati on 10/30/19.
//

#include "../include/directory_parser.h"

/** Constructor
 *  @param directory path
 */
DirectoryParser::DirectoryParser(const char *path) {
    if ((this->dir = opendir(path)) == nullptr)
        throw "Invalid directory\n";

}

/** Constructor
 *  @param string vector
 */
void DirectoryParser::read_directory(std::vector<std::string> &v) {
    struct dirent *ent;
    while ((ent = readdir(this->dir)) != nullptr) {
        if (ent->d_type != DT_DIR)
            v.emplace_back(ent->d_name);
    }
}

DirectoryParser::~DirectoryParser() {
    closedir(dir);
}

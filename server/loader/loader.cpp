//
// Created by mati on 30/10/19.
//

#include "loader.h"

Loader::Loader(const char *path) : parser(path) {}

void Loader::load_dynamic_libraries() {
    std::vector<std::string> dir;
    this->parser.read_directory(dir);

    for (int i = 0; i < (int) dir.size(); ++i) {
        std::string lib_name = "../server/loader/libs/" + dir[i];
        void *handle;
        handle = dlopen(lib_name.c_str(), RTLD_NOW);

        if (!handle) {
            printf("The error is %s", dlerror());
            return;
        }

        typedef TestVir *execute_t();

        execute_t *execute = (execute_t *) dlsym(handle, "execute");

        if (!execute) printf("The error is %s\n", dlerror());

        execute();
    }
}

Loader::~Loader() {}

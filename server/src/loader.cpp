//
// Created by mati on 30/10/19.
//

#define LIBS_PATH "../server/include/libs/so"

#include "../include/loader.h"

Loader::Loader() : parser(LIBS_PATH) {}

void Loader::load_dynamic_libraries() {
    std::vector<std::string> dir;
    this->parser.read_directory(dir);

    for (const auto &i : dir) {
        void *handle;
        std::string lib_name = LIBS_PATH + i;
        handle = dlopen(lib_name.c_str(), RTLD_NOW);

        if (!handle) {
            printf("The error is %s", dlerror());
            continue;
        }

        auto *execute = (execute_t *) dlsym(handle, "execute");

        if (!execute) {
            printf("The error is %s\n", dlerror());
            continue;
        }

        this->functors.emplace_back(execute);
    }
}

void Loader::execute() {
    for (auto &functor : this->functors)
        functor();
}

Loader::~Loader() = default;

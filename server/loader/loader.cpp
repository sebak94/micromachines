//
// Created by mati on 30/10/19.
//

#include "loader.h"

Loader::Loader(const char *path) : parser(path) {}

void Loader::load_dynamic_libraries() {
    std::vector<std::string> dir;
    this->parser.read_directory(dir);
    for (int i = 0; i < (int) dir.size(); ++i) {
        printf("vec[%i] = %s\n", i, dir[i].c_str());
    }

    std::string lib_name = "./" + dir[0];
    void *handle;
    handle = dlopen(lib_name.c_str(), RTLD_NOW);

    if (!handle) printf("The error is %s", dlerror());

    typedef TestVir *create_t();
    typedef void destroy_t(TestVir *);

    create_t *creat = (create_t *) dlsym(handle, "create");
    destroy_t *destroy = (destroy_t *) dlsym(handle, "destroy");

    if (!creat || !destroy) printf("The error is %s\n", dlerror());

    TestVir *tst = creat();
    tst->execute();
    destroy(tst);
}

Loader::~Loader() {}

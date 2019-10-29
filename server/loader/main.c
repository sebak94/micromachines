// Armar el ejecutable
//  gcc -Wall -Werror -std=c99 -pedantic -ggdb -o main  main.c -ldl

#include <dlfcn.h>
#include <stdio.h>

typedef int (*func_pointer)(int);

int print_error_and_close(void *shared_lib, const char *msj, const char *err) {
    printf("%s: %s\n", msj, err);
    dlclose(shared_lib);
    return -1;
}

void *load_lib(const char *lib_name) {
    void *shared_lib = dlopen(lib_name, RTLD_NOW);
    if (!shared_lib)
        printf("dlopen failed: %s\n", dlerror());
    return shared_lib;
}

int main(int argc, char *argv[]) {
    void *shared_lib = load_lib(argv[1]);
    if (!shared_lib) return -1;

    func_pointer execute;
    *(void **) (&execute) = dlsym(shared_lib, "execute");

    const char *err = dlerror();
    if (err)
        return print_error_and_close(
                shared_lib, " execute failed", dlerror());

    int result = execute(3);
    printf("result:%i.\n", result);

    //libero el recurso luego de su uso.
    dlclose(shared_lib);

    return result;
}

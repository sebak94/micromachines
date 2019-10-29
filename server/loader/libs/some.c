//  Compilar la libreria generando codigo cuyos offsets son independientes
//  gcc -Wall -Werror -std=c99 -pedantic -fPIC -shared -o some.so some.c

int execute(int i) {
    return i * 2;
}


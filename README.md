# Micromachines

## Compilación
- Instalar CMake https://cmake.org/install/
- Crear la carpeta `/build`
```
mkdir build
cd build
```
- Ejecutar cmake pasándole el directorio donde se encuentra `CMakeLists.txt`
```
cmake ../
```
- Ejecutar `make`
```
make
```

## Ejecución
Luego de compilar se deben haber creado 2 archivos para ejecutar:
- `micromachines-sv`
- `micromachines-cl`

### Servidor
Ej:
```
./micromachines-sv 3000
```

### Cliente
Ej:
```
./micromachines-cl localhost 3000
```

### JSONCPP
* Instalar:
```
sudo apt-get install libjsoncpp-dev
```
* Cmake:
```
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
add_executable(map main.cpp)
```

***NOTA: Cualquier modificación de estos pasos hacer el cambio correspondiente en este Readme.***

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

## Bibliotecas
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
```

### SDL
* Instalar (server):
```
sudo apt-get install libsdl2-dev
```

* Instalar (client):
```
sudo apt-get install li
sudo apt-get install libavformat-dev
sudo apt-get install libavformat57
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-mixer-dev
sudo apt-get install libswscale-dev
sudo apt install libdvdnav4 libdvdread4 gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly libdvd-pkg
sudo apt install ubuntu-restricted-extras
```

* Cmake:
```
target_link_libraries(main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        swscale)
```
***NOTA: Cualquier modificación de estos pasos hacer el cambio correspondiente en este Readme.***

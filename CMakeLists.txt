cmake_minimum_required(VERSION 3.10)
project(tp)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -pedantic -pthread")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_executable(tp
        main.cpp
        Car.cpp Car.h
        Window.cpp Window.h
        Error.cpp Error.h
        Button.cpp Button.h
        Record.cpp Record.h)

target_link_libraries(tp
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        avutil
        avformat
        swscale
        avcodec)
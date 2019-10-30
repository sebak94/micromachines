cmake_minimum_required(VERSION 2.8)

project(Micromachines)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES_SERVER
    server/include/acceptor_th.h
    server/src/acceptor_th.cpp
    server/include/client_th.h
    server/src/client_th.cpp
    common/include/socket.h
    common/src/socket.cpp
    common/include/socket_error.h
    common/src/socket_error.cpp
    common/include/thread.h
    common/src/thread.cpp
        common/include/directory_parser.h
        common/src/directory_parser.cpp
)

set(SOURCE_FILES_CLIENT
    client/include/server.h
    client/src/server.cpp
    common/include/socket.h
    common/src/socket.cpp
    common/include/socket_error.h
    common/src/socket_error.cpp
    common/include/thread.h
    common/src/thread.cpp
)

add_executable(micromachines-sv ${SOURCE_FILES_SERVER} server/main.cpp)
add_executable(micromachines-cl ${SOURCE_FILES_CLIENT} client/main.cpp)

target_link_libraries(micromachines-cl SDL2 SDL2_image)

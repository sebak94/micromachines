cmake_minimum_required(VERSION 2.8)

project(Micromachines)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES_SERVER
    server/include/mm_server.h
    server/include/acceptor_th.h
    server/include/client_th.h
    server/include/game_loop_th.h
    server/include/blocking_queue.h
    server/include/model/micromachines.h
    server/include/model/model.h
    server/include/model/point.h
    server/include/model/interpreter.h
    server/include/model/cars/color.h
    server/include/model/cars/car.h
    server/include/model/cars/blue_car.h
    server/include/model/cars/states/car_state.h
    server/include/model/cars/states/accelerate_left.h
    server/include/model/cars/states/accelerate_right.h
    server/include/model/cars/states/accelerate.h
    server/include/model/cars/states/break_left.h
    server/include/model/cars/states/break_right.h
    server/include/model/cars/states/break.h
    server/include/model/cars/states/left.h
    server/include/model/cars/states/right.h
    server/include/model/cars/states/nothing.h
    server/include/model/tracks/track.h
    server/include/model/tracks/track_part_data.h
    server/include/model/tracks/track_parts.h
    server/src/mm_server.cpp
    server/src/acceptor_th.cpp
    server/src/client_th.cpp
    server/src/game_loop_th.cpp
    server/src/blocking_queue.cpp
    server/src/model/micromachines.cpp
    server/src/model/point.cpp
    server/src/model/point.cpp
    server/src/model/interpreter.cpp
    server/src/model/tracks/track.cpp
    server/src/model/cars/color.cpp
    server/src/model/cars/car.cpp
    server/src/model/cars/blue_car.cpp
    server/src/model/cars/states/accelerate_left.cpp
    server/src/model/cars/states/accelerate_right.cpp
    server/src/model/cars/states/accelerate.cpp
    server/src/model/cars/states/break_left.cpp
    server/src/model/cars/states/break_right.cpp
    server/src/model/cars/states/break.cpp
    server/src/model/cars/states/left.cpp
    server/src/model/cars/states/right.cpp
    server/src/model/cars/states/nothing.cpp
    common/include/socket.h
    common/include/socket_error.h
    common/include/thread.h
    common/include/lock.h
    common/src/socket.cpp
    common/src/socket_error.cpp
    common/src/thread.cpp
    common/src/lock.cpp
)

set(SOURCE_FILES_CLIENT
    client/include/server.h
    client/src/server.cpp
    common/include/socket.h
    common/include/socket_error.h
    common/include/thread.h
    common/src/socket.cpp
    common/src/socket_error.cpp
    common/src/thread.cpp
)

add_executable(micromachines-sv ${SOURCE_FILES_SERVER} server/main.cpp)
add_executable(micromachines-cl ${SOURCE_FILES_CLIENT} client/main.cpp)

target_link_libraries(micromachines-cl SDL2 SDL2_image)

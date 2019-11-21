cmake_minimum_required(VERSION 3.5)
project(Micromachines)

set(CMAKE_CXX_STANDARD 11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES_SERVER
    server/include/mm_server.h
    server/include/acceptor_th.h
    server/include/client_th.h
    server/include/game_loop_th.h
    server/include/model/micromachines.h
    server/include/model/model.h
    server/include/model/point.h
    server/include/model/cars/color.h
    server/include/model/cars/car.h
    server/include/model/cars/blue_car.h
	server/include/model/cars/red_car.h
    server/include/model/cars/white_car.h
    server/include/model/cars/black_car.h
    server/include/model/cars/yellow_car.h
	server/include/model/box2d-entities/destruction_listener.h
	server/include/model/box2d-entities/fixture_user_data.h
	server/include/model/box2d-entities/tdcar.h
	server/include/model/box2d-entities/tdtire.h
    server/src/mm_server.cpp
    server/src/acceptor_th.cpp
    server/src/client_th.cpp
    server/src/game_loop_th.cpp
    server/src/model/micromachines.cpp
    server/src/model/point.cpp
    server/src/model/point.cpp
    server/src/model/cars/color.cpp
    server/src/model/cars/car.cpp
    server/src/model/cars/blue_car.cpp
	server/src/model/cars/red_car.cpp
    server/src/model/cars/white_car.cpp
    server/src/model/cars/black_car.cpp
    server/src/model/cars/yellow_car.cpp
	server/src/model/box2d-entities/tdcar.cpp
	server/src/model/box2d-entities/tdtire.cpp
    common/include/socket.h
    common/include/socket_error.h
    common/include/thread.h
    common/include/lock.h
    common/include/Track.h
    common/include/TrackList.h
    common/include/TrackPartData.h
    common/src/socket.cpp
    common/src/socket_error.cpp
    common/src/thread.cpp
    common/src/lock.cpp
    common/src/Track.cpp
    common/src/TrackList.cpp
    common/src/TrackPartData.cpp
    common/include/directory_parser.h
    common/src/directory_parser.cpp
    server/include/loader.h
    server/src/loader.cpp
    common/src/Grandstand.cpp
    common/include/Grandstand.h
    common/include/directory_parser.h
    common/src/directory_parser.cpp
    server/include/loader.h
    server/src/loader.cpp
)

set(SOURCE_FILES_CLIENT
    client/include/server.h
    client/src/server.cpp
	client/include/sdl/SdlAnimation.h
	client/src/sdl/SdlAnimation.cpp
	client/include/sdl/SdlException.h
	client/src/sdl/SdlException.cpp
	client/include/sdl/SdlSurface.h
	client/src/sdl/SdlSurface.cpp
	client/include/sdl/SdlTexture.h
	client/src/sdl/SdlTexture.cpp
	client/include/sdl/SdlWindow.h
	client/src/sdl/SdlWindow.cpp
	client/include/sdl/SdlMusic.h
	client/src/sdl/SdlMusic.cpp
	client/include/PictureLoader.h
    client/src/PictureLoader.cpp
	client/include/ThreadSafeQueue.h
	client/src/ThreadSafeQueue.cpp
	client/include/lua/ai.h
	client/src/lua/ai.cpp
	client/include/EventLoopSDL.h
	client/src/EventLoopSDL.cpp
	client/include/EventLoopIA.h
	client/src/EventLoopIA.cpp
	client/include/Dispatcher.h
	client/src/Dispatcher.cpp
	client/include/Drawer.h
	client/src/Drawer.cpp
    client/include/Car.h
	client/src/Car.cpp
	client/include/Model.h
	client/src/Model.cpp
	client/include/Camera.h
	client/src/Camera.cpp
	client/include/ModelUpdater.h
	client/src/ModelUpdater.cpp
	client/include/ModelMonitor.h
	client/src/ModelMonitor.cpp
	common/include/socket.h
    common/include/socket_error.h
    common/include/thread.h
    common/include/Track.h
    common/include/TrackList.h
    common/include/TrackPartData.h
    common/src/socket.cpp
    common/src/socket_error.cpp
    common/src/thread.cpp
    common/src/Track.cpp
    common/src/TrackList.cpp
    common/src/TrackPartData.cpp
	common/src/Grandstand.cpp
	common/include/Grandstand.h
	common/include/Button.h
	common/src/Button.cpp
    server/src/model/point.cpp
	record/include/Record.h
	record/src/Record.cpp
	common/include/Error.h
	common/src/Error.cpp
	client/src/MatchWindow.cpp
	client/include/MatchWindow.h
	common/src/TextTexture.cpp
	common/include/TextTexture.h
	)

set(SOURCE_FILES_EDITOR
	common/src/TrackList.cpp
	common/include/TrackList.h
	common/src/Track.cpp
	common/include/Track.h
	common/src/TrackPartData.cpp
	common/include/TrackPartData.h
	editor/src/TrackEditor.cpp
	editor/include/TrackEditor.h
	editor/src/Window.cpp
	editor/include/Window.h
	common/src/Button.cpp
	common/include/Button.h
	common/src/TextTexture.cpp
	common/include/TextTexture.h
	common/src/Prompt.cpp
	common/include/Prompt.h
	editor/src/TrackBlock.cpp
	editor/include/TrackBlock.h
	editor/src/TrackGrid.cpp
	editor/include/TrackGrid.h
	common/src/Grandstand.cpp
	common/include/Grandstand.h
	server/src/model/point.cpp
	server/src/model/point.cpp
	common/src/Error.cpp
	common/include/Error.h)

add_executable(micromachines-sv ${SOURCE_FILES_SERVER} server/main.cpp)
add_executable(micromachines-cl client/src/lua/ai.o ${SOURCE_FILES_CLIENT}
		client/main.cpp)
add_executable(micromachines-ed ${SOURCE_FILES_EDITOR} editor/main.cpp)

target_link_libraries(micromachines-ed
		SDL2
		SDL2_image
		SDL2_mixer
		SDL2_ttf
		swscale)

target_link_libraries(micromachines-cl
		SDL2
		SDL2_image
		SDL2_mixer
        SDL2_ttf
        avutil
        avformat
        swscale
        avcodec
        lua5.3)

target_link_libraries(micromachines-sv -ldl -lBox2D)

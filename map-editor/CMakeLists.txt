cmake_minimum_required(VERSION 3.15)
project(map)

set(CMAKE_CXX_STANDARD 11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
add_executable(map main.cpp TrackList.cpp TrackList.h Track.cpp Track.h TrackPartData.cpp TrackPartData.h TrackEditor.cpp TrackEditor.h)

target_link_libraries(map ${Jsoncpp_LIBRARY})

add_executable(mapEditor mainEditor.cpp
        TrackList.cpp TrackList.h
        Track.cpp Track.h
        TrackPartData.cpp TrackPartData.h
        TrackEditor.cpp TrackEditor.h
        Window.cpp Window.h
        Error.cpp Error.h
        Button.cpp Button.h TextTexture.cpp TextTexture.h Prompt.cpp Prompt.h TrackBlock.cpp TrackBlock.h TrackGrid.cpp TrackGrid.h)

target_link_libraries(mapEditor
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        swscale)
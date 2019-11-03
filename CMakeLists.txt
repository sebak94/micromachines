cmake_minimum_required(VERSION 3.15)
project(map)

set(CMAKE_CXX_STANDARD 11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
add_executable(map main.cpp common/src/TrackList.cpp common/include/TrackList.h common/src/Track.cpp common/include/Track.h common/src/TrackPartData.cpp common/include/TrackPartData.h)

target_link_libraries(map ${Jsoncpp_LIBRARY})
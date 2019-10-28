cmake_minimum_required(VERSION 2.6)
project(lua-examples)

find_package(Lua REQUIRED)

message("Variables:\n")
message("LUA_FOUND: ${LUA_FOUND}")
message("LUA_LIBRARIES: ${LUA_INCLUDE_DIR}")
message("LUA_INCLUDE_DIR: ${LUA_INCLUDE_DIR}")
message("LUA_VERSION_STRING: ${LUA_VERSION_STRING}")
message("LUA_VERSION_MAJOR: ${LUA_VERSION_MAJOR}")
message("LUA_VERSION_MINOR: ${LUA_VERSION_MINOR}")
message("LUA_VERSION_PATCH: ${LUA_VERSION_PATCH}")

include_directories(${LUA_INCLUDE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pedantic")
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(src)

install(FILES src/ia.lua DESTINATION
        ${CMAKE_BINARY_DIR})

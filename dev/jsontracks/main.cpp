// %https://github.com/open-source-parsers/jsoncpp
// https://www.codeproject.com/Articles/1102603/Accessing-JSON-Data-with-Cplusplus
// https://github.com/open-source-parsers/jsoncpp/issues/455
// https://en.wikibooks.org/wiki/JsonCpp
/* Instalar con:
 * sudo apt-get install libjsoncpp-dev
 * y en CMake:
 * find_package(PkgConfig REQUIRED)
 * pkg_check_modules(JSONCPP jsoncpp)
 * link_libraries(${JSONCPP_LIBRARIES})
 * add_executable(map main.cpp)
 * */

#include "TrackList.h"

int main() {
    TrackList tracks;

    tracks.readTracks();
    tracks.printTrack("classic");
    tracks.printTrack("mini");
    tracks.printTrack("pro");
    Track proTrack = tracks.getTrack("pro");
    proTrack.print();
    std::cout << "Track element enumeration in (1,2) is: ";
    std::cout << proTrack.getElem(1,2) << std::endl;

    return 0;
}
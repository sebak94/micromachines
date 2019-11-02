#include "TrackList.h"

int main() {
    TrackList tracks;

    tracks.readTracks();
    tracks.printTrack("classic");
    tracks.printTrack("mini");
    tracks.printTrack("pro");
    Track mini = tracks.getTrack("mini");
    mini.print();
    std::cout << "Track element enumeration in (1,2) is: ";
    std::cout << mini.getElemType(1, 2) << std::endl;

    std::cout << mini.isOnTrack(105, 135);
    mini.printElem(mini.getTrackPart(205,105));

    return 0;
}
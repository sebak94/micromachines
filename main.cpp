#include "TrackList.h"

int main() {
    TrackList tracks;

    //test load
    tracks.readTracks();
    tracks.printTrack("classic");
    tracks.printTrack("mini");
    tracks.printTrack("pro");
    Track mini = tracks.getTrack("mini");
    mini.print();

    // test enumeration
    std::cout << "Track element enumeration in (1,2) is: ";
    std::cout << mini.getPartType(1, 2) << std::endl;

    // test positions
    std::cout << mini.isOnTrack(105, 135);
    mini.printElem(mini.getTrackPart(205,105));

    //test serialize and print received
    Track pro = tracks.getTrack("pro");
    std::cout << pro.serialize() << std::endl;
    Track received(pro.serialize());
    std::cout << received.getName() << std::endl;
    received.print();

    return 0;
}
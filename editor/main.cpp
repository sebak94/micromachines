#include "include/Window.h"
#include "include/TrackEditor.h"

int main() {
    TrackEditor editor;
    Window game;

    try {
        editor.run(game);
    } catch (Error & e) {
        e.printError();
    }
    return 0;
}

#include "include/Window.h"
#include "include/TrackEditor.h"

int main() {
    TrackEditor editor;
    Window window;

    try {
        editor.run(window);
    } catch (Error & e) {
        e.printError();
    }
    return 0;
}

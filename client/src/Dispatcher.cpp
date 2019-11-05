#include "../include/Dispatcher.h"
#include "../include/Drawer.h"

Dispatcher::Dispatcher(ThreadSafeQueue &queue, Socket &socket) : queue(queue), socket(socket) {}

Dispatcher::~Dispatcher() {}

void Dispatcher::run() {
    this->running = true;
    while (running) {
        std::string text = this->queue.pop();
        if (text == "Q") {
            running = false;
        }
        try {
            printf("envio: %s\n", text.c_str());
            socket.Send(text.c_str(), text.size());
        } catch (std::exception &e) {
            printf("murio el socket en el dispatcher\n");
            running = false;
        }
    }
}

void Dispatcher::stop() {

}

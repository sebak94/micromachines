#include "../include/Dispatcher.h"
#include "../include/Drawer.h"

Dispatcher::Dispatcher(ThreadSafeQueue &queue, Socket &socket) : queue(queue), socket(socket) {}

Dispatcher::~Dispatcher() {}

void Dispatcher::run() {
    this->running = true;
    while (running) {
        std::string text = this->queue.pop();
        std::cout << "envio: " << text << std::endl;
        if (text == "Q") {
            running = false;
        }
        try {
            socket.Send(text.c_str(), text.size());
            //printf("envio: %s\n", text.c_str());
        } catch (std::exception &e) {
            printf("Dispatcher::run() exception catched: %s\n", e.what());
            running = false;
        }
    }
    socket.Release();
}

void Dispatcher::stop() {

}

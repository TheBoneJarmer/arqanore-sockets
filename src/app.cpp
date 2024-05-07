#include <iostream>
#include "arqanore/window.h"
#include "arqanore/keyboard.h"
#include "arqanore/exceptions.h"
#include "arqanore/arqsocket.h"
#include "arqanore/arqmessage.h"

using namespace std;
using namespace arqanore;

void on_open(Window *window) {
    
}

void on_close(Window *window) {
    
}

void on_tick(Window *window, double dt) {

}

std::string send(std::string action) {
    auto msg = ArqMessage();
    msg.action = action;
    msg.body = "null";

    std::string res;

    try {
        char buffer[1024];

        auto socket = ArqSocket();
        socket.read_timeout = 1000;

        socket.connect("localhost", 9091);
        socket.send(msg.to_string());

        auto read = socket.read(buffer, 1024);
        res = std::string(buffer, read);

        socket.close();
    } catch (ArqanoreException &ex) {
        cerr << ex.what() << endl;
    }

    return res;
}

void on_update(Window *window, double at) {
    if (Keyboard::key_pressed(Keys::ESCAPE)) {
        window->close();
    }

    if (Keyboard::key_pressed(Keys::ENTER)) {
        auto res = send("ping");

        if (res.empty()) {
            cerr << "Response is empty" << endl;
        } else {
            cout << res << endl;
        }
    }
}

void on_render2d(Window *window) {
    try {

    } catch (ArqanoreException& ex) {
        cerr << ex.what() << endl;
        window->close();
    }
}

void on_render3d(Window *window) {

}

void on_char(Window *window, unsigned int codepoint) {

}

void on_opengl(Window *window, string type, string severity, string message) {
    if (type != "ERROR") {
        return;
    }

    cout << "[" << type << "][" << severity << "] " << message << endl;
}

int main() {
    try {
        auto window = Window(1440, 786, "App");
        window.on_open(on_open);
        window.on_close(on_close);
        window.on_tick(on_tick);
        window.on_update(on_update);
        window.on_render2d(on_render2d);
        window.on_render3d(on_render3d);
        window.on_char(on_char);
        window.on_opengl(on_opengl);
        window.open(false, true);
    } catch (ArqanoreException &ex) {
        cerr << ex.what() << endl;
        return 1;
    }

    return 0;
}

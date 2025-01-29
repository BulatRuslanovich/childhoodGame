//
// Created by getname on 27.01.2025.
//

#include "Events.h"

#include <cstring>
#include <GLFW/glfw3.h>

#define MOUSE_BUTTON 1024

bool* Events::_keys;
bool Events::_cursor_locked = false;
bool Events::_cursor_started = false;
uint* Events::_frames;
uint Events::_current = 0;
float Events::x = 0.0f;
float Events::y = 0.0f;
float Events::dx = 0.0f;
float Events::dy = 0.0f;

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mode) {
    if ( action == GLFW_PRESS ) {
        Events::_keys[key] = true;
        Events::_frames[key] = Events::_current;
    } else if (action == GLFW_RELEASE) {
        Events::_keys[key] = false;
        Events::_frames[key] = Events::_current;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
    if ( action == GLFW_PRESS ) {
        Events::_keys[MOUSE_BUTTON + button] = true;
        Events::_frames[MOUSE_BUTTON + button] = Events::_current;
    } else if (action == GLFW_RELEASE) {
        Events::_keys[MOUSE_BUTTON + button] = false;
        Events::_frames[MOUSE_BUTTON + button] = Events::_current;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if ( Events::_cursor_started ) {
        Events::dx += xpos - Events::x;
        Events::dy += ypos - Events::y;
    } else {
        Events::_cursor_started = true;
    }

    Events::x = xpos;
    Events::y = ypos;
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window::height = height;
    Window::width = width;
}


int Events::init() {
    GLFWwindow* window = Window::window;
    _keys = new bool[1032];
    _frames = new uint[1032];

    memset(_keys, false, sizeof(bool) * 1032);
    memset(_frames, 0, sizeof(uint) * 1032);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    return 0;
}

void Events::pullEvents() {
    dx = 0.0f;
    dy = 0.0f;
    _current++;
    glfwPollEvents();
}

bool Events::pressed(const int key) {
    return key < 0 || key >= MOUSE_BUTTON ? false : _keys[key];
}

bool Events::jpressed(const int key) {
    return key < 0 || key >= MOUSE_BUTTON ? false : _keys[key] && _frames[key] == _current;
}

bool Events::clicked(const int button) {
    return _keys[MOUSE_BUTTON + button];
}

bool Events::jclicked(const int button) {
    return _keys[MOUSE_BUTTON + button] && _frames[MOUSE_BUTTON + button] == _current;
}

void Events::toogleCursor() {
    _cursor_locked = !_cursor_locked;
    Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

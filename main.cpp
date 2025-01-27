
#define GLEW_STATIC
#include <GLFW/glfw3.h>

#include "window/Events.h"
#include "window/Window.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main()
{
    Window::init(WINDOW_WIDTH, WINDOW_HEIGHT, "LOL");
    Events::init();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!Window::isShouldClose()) {
        Events::pullEvents();
        if (Events::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }

        if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        Window::swapBuffers();
    }

    Window::terminate();

    return 0;
}

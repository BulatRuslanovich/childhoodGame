//
// Created by getname on 27.01.2025.
//

#ifndef WINDOW_H
#define WINDOW_H

class GLFWwindow;

class Window {
    public:
    static int width;
    static int height;
    static GLFWwindow* window;

    static int init(int width, int height, const char* title);
    static void terminate();

    static void setCursorMode(int mode);
    static bool isShouldClose();
    static void swapBuffers();
    static void setShouldClose(bool flag);

};



#endif //WINDOW_H

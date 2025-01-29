//
// Created by getname on 27.01.2025.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"

GLFWwindow* Window::window;
int Window::width;
int Window::height;

int Window::init(const int width, const int height, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        std::cout << "Failed to create GLFW window." << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, width, height);

    Window::height = height;
    Window::width = width;
    return 0;
}

void Window::terminate() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setCursorMode(const int mode) {
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

bool Window::isShouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::setShouldClose(const bool flag) {
    glfwSetWindowShouldClose(window, flag);
}


#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/Shader.h"
#include "window/Events.h"
#include "window/Window.h"
#include "loader/PngLoader.h"
#include "graphics/Texture.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

float vertices[] = {
    // x    y     z     u     v
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

int main() {
    Window::init(WINDOW_WIDTH, WINDOW_HEIGHT, "LOL");
    Events::init();

    const Shader* shader = loadShader("res/main.glslv", "res/main.glslf");
    if ( shader == nullptr) {
        std::cerr << "Failed to load shaders" << std::endl;
        delete shader;
        Window::terminate();
        return -1;
    }

    const Texture *texture = loadTexture("res/lol.png");
    if (texture == nullptr) {
        std::cerr << "Failed to load texture" << std::endl;
        delete shader;
        delete texture;
        Window::terminate();
        return -1;
    }

    //Create VAO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!Window::isShouldClose()) {
        Events::pullEvents();

        if (Events::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }

        if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // Draw VAO
        shader->use();
        texture->bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        Window::swapBuffers();
    }

    Window::terminate();
    delete shader;
    delete texture;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    return 0;
}

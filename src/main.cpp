
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "window/Camera.h"
using namespace glm;

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

    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    const auto camera = new Camera(vec3(0, 0, 1), radians(90.0f));

    mat4 model(1.0f);
    model = translate(model, vec3(0.5f, 0.0f, 0.0f));

    float lastTime = glfwGetTime();
    float deltaTime = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    constexpr float speed = 5.0f;

    while (!Window::isShouldClose()) {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        if (Events::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }

        if (Events::jpressed(GLFW_KEY_TAB)) {
            Events::toogleCursor();
        }

        if (Events::pressed(GLFW_KEY_W)) {
            camera->pos += camera->front * deltaTime * speed;
        }

        if (Events::pressed(GLFW_KEY_S)) {
            camera->pos -= camera->front * deltaTime * speed;
        }

        if (Events::pressed(GLFW_KEY_D)) {
            camera->pos += camera->right * deltaTime * speed;
        }

        if (Events::pressed(GLFW_KEY_A)) {
            camera->pos -= camera->right * deltaTime * speed;
        }

        if (Events::_cursor_locked) {
            camY += -Events::dy / Window::height * 2;
            camX += -Events::dx / Window::height * 2;

            if (camY < -radians(89.0f)) {
                camY = -radians(89.0f);
            }

            if (camX > radians(89.0f)) {
                camX = radians(89.0f);
            }

            camera->rotation = mat4(1.0f);
            camera->rotate(camY, camX, 0.0f);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // Draw VAO
        shader->use();
        shader->uniformMatrix("model", model);
        shader->uniformMatrix("projview", camera->getProjection() * camera->getView());
        texture->bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        Window::swapBuffers();
        Events::pullEvents();
    }

    delete shader;
    delete texture;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    Window::terminate();

    return 0;
}

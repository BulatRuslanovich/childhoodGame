// Основные заголовочные файлы для ввода/вывода и работы с памятью
#include <iostream>

// Настройки для графических библиотек
#define GLEW_STATIC // Используем статическую линковку для GLEW
#include <GL/glew.h> // Основная графическая библиотека (OpenGL)
#include <GLFW/glfw3.h> // Библиотека для создания окон и обработки ввода

// Математическая библиотека для 3D-преобразований
#include <glm/glm.hpp> // Основные типы (vec3, mat4 и т.д.)
#include <glm/ext.hpp> // Дополнительные функции (радианы, трансформации)

// Наши собственные заголовочные файлы
#include "graphics/Mesh.h"
#include "window/Camera.h"

#include "graphics/Shader.h"
#include "window/Events.h"
#include "window/Window.h"
#include "loader/PngLoader.h"
#include "graphics/Texture.h"
#include "graphics/VoxelRenderer.h"
#include "voxels/Chunk.h"

// Константы для размеров окна
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Вершины для отрисовки прямоугольника (2 треугольника)
// Каждая вершина содержит: X, Y, Z (позиция) и U, V (текстурные координаты)
float vertices[] = {
    // x      y      z     u     v
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Левый нижний угол
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Правый нижний угол
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // Левый верхний угол

     1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Повтор для второго треугольника
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // Правый верхний угол
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f  // Левый верхний угол (снова)
};

int attrs[] = {
    3, 2, 0
};

int main() {
    // Инициализация окна и системы событий
    Window::init(WINDOW_WIDTH, WINDOW_HEIGHT, "LOL");
    Events::init();

    // Загрузка шейдеров (программ для видеокарты)
    const Shader* shader = loadShader("res/main.glslv", "res/main.glslf");
    if (shader == nullptr) {
        std::cerr << "Ошибка загрузки шейдеров!" << std::endl;
        Window::terminate();
        return -1;
    }

    // Загрузка текстуры (изображения для наложения на объекты)
    const Texture* texture = loadTexture("res/block.png");
    if (texture == nullptr) {
        std::cerr << "Ошибка загрузки текстуры!" << std::endl;
        delete shader;
        Window::terminate();
        return -1;
    }

    // Настройка графического состояния
    // Цвет очистки экрана (черный)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Включение прозрачности
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    // Формула смешивания цветов
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto camera = new Camera(glm::vec3(0, 0, 20), glm::radians(90.0f));
    const VoxelRenderer renderer(1024 * 1024 * 8);
    const auto chunk = new Chunk();
    const auto mesh = renderer.render(chunk);

    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(0.5f,0,0));

    float lastTime = glfwGetTime();
    float deltaTime = 0.0f;
    float camX = 0.0f, camY = 0.0f;

    while (!Window::isShouldClose()) {
        constexpr float speed = 5.0f;
        // Расчет времени между кадрами
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        // Обработка ввода (клавиатура)
        if (Events::jpressed(GLFW_KEY_ESCAPE))
            Window::setShouldClose(true);

        if (Events::jpressed(GLFW_KEY_TAB))
            Events::toogleCursor();

        if (Events::pressed(GLFW_KEY_W))
            camera->pos += camera->front * deltaTime * speed;
        if (Events::pressed(GLFW_KEY_S))
            camera->pos -= camera->front * deltaTime * speed;
        if (Events::pressed(GLFW_KEY_D))
            camera->pos += camera->right * deltaTime * speed;
        if (Events::pressed(GLFW_KEY_A))
            camera->pos -= camera->right * deltaTime * speed;

        // Обработка движения мыши (вращение камеры)
        if (Events::_cursor_locked) {
            camY += -Events::dy / Window::height * 2;
            camX += -Events::dx / Window::height * 2;

            // Ограничение углов поворота
            camY = glm::clamp(camY, -glm::radians(89.0f), glm::radians(89.0f));

            // Обновление матрицы вращения камеры
            camera->rotation = glm::mat4(1.0f);
            camera->rotate(camY, camX, 0.0f);
        }

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Рендеринг объектов
        shader->use();
        shader->uniformMatrix("model", model);
        shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
        texture->bind();
        mesh->draw(GL_TRIANGLES);
        Window::swapBuffers();
        Events::pullEvents();
    }

    // Очистка ресурсов
    delete shader;
    delete texture;
    delete camera;
    delete mesh;
    delete chunk;
    Window::terminate();

    return 0;
}

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
#include "window/Camera.h"
using namespace glm; // Используем пространство имен glm для математических типов

#include "graphics/Shader.h"
#include "window/Events.h"
#include "window/Window.h"
#include "loader/PngLoader.h"
#include "graphics/Texture.h"

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

int main() {
    // Инициализация окна и системы событий
    Window::init(WINDOW_WIDTH, WINDOW_HEIGHT, "LOL"); // Создаем окно
    Events::init(); // Настраиваем обработку ввода

    // Загрузка шейдеров (программ для видеокарты)
    const Shader* shader = loadShader("res/main.glslv", "res/main.glslf");
    if (shader == nullptr) {
        std::cerr << "Ошибка загрузки шейдеров!" << std::endl;
        Window::terminate(); // Закрываем GLFW перед выходом
        return -1;
    }

    // Загрузка текстуры (изображения для наложения на объекты)
    const Texture* texture = loadTexture("res/lol.png");
    if (texture == nullptr) {
        std::cerr << "Ошибка загрузки текстуры!" << std::endl;
        delete shader;     // Удаляем шейдеры
        Window::terminate();
        return -1;
    }

    // Настройка графического конвейера (VAO/VBO)
    // VAO - Vertex Array Object (контейнер для настроек вершин)
    // VBO - Vertex Buffer Object (буфер с данными вершин)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);  // Создаем VAO
    glGenBuffers(1, &VBO);       // Создаем VBO

    // Привязываем VAO и VBO для настройки
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Копируем данные вершин в видеопамять
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Настройка атрибутов вершин:
    // 0-й атрибут: позиция (X, Y, Z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // 1-й атрибут: текстурные координаты (U, V)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Отвязываем VAO

    // Настройка графического состояния
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Цвет очистки экрана (черный)
    glEnable(GL_BLEND);                   // Включение прозрачности
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Формула смешивания цветов

    // Создание камеры (позиция, угол обзора)
    Camera* camera = new Camera(vec3(0, 0, 1), radians(90.0f));

    // Основные переменные для работы цикла
    float lastTime = glfwGetTime(); // Для расчета времени между кадрами
    float deltaTime = 0.0f;         // Время между кадрами (в секундах)
    float camX = 0.0f, camY = 0.0f; // Углы поворота камеры
    constexpr float speed = 5.0f;   // Скорость движения камеры

    // Главный цикл приложения
    while (!Window::isShouldClose()) {
        // Расчет времени между кадрами
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        // Обработка ввода (клавиатура)
        if (Events::jpressed(GLFW_KEY_ESCAPE)) // Нажатие ESC - выход
            Window::setShouldClose(true);

        if (Events::jpressed(GLFW_KEY_TAB))    // TAB - переключение курсора
            Events::toogleCursor();

        // Движение камеры с помощью WASD
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
            camY += -Events::dy / Window::height * 2; // Вертикальный поворот
            camX += -Events::dx / Window::height * 2; // Горизонтальный поворот

            // Ограничение углов поворота
            camY = glm::clamp(camY, -glm::radians(89.0f), glm::radians(89.0f));

            // Обновление матрицы вращения камеры
            camera->rotation = mat4(1.0f);
            camera->rotate(camY, camX, 0.0f);
        }

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT);

        // Рендеринг объектов
        shader->use(); // Активация шейдерной программы

        // Передача матриц в шейдер
        shader->uniformMatrix("model", glm::translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.0f)));
        shader->uniformMatrix("projview", camera->getProjection() * camera->getView());

        texture->bind();         // Привязка текстуры
        glBindVertexArray(VAO);  // Привязка вершинных данных
        glDrawArrays(GL_TRIANGLES, 0, 6); // Отрисовка 6 вершин (2 треугольника)
        glBindVertexArray(0);    // Отвязка VAO

        // Обновление окна и обработка событий
        Window::swapBuffers(); // Переключение буферов (двойная буферизация)
        Events::pullEvents();  // Обработка новых событий ввода
    }

    // Очистка ресурсов
    delete shader;    // Удаление шейдеров
    delete texture;   // Удаление текстуры
    delete camera;    // Удаление камеры
    glDeleteBuffers(1, &VBO);      // Удаление VBO
    glDeleteVertexArrays(1, &VAO); // Удаление VAO
    Window::terminate();           // Завершение работы GLFW

    return 0;
}

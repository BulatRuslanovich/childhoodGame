//
// Файл создан getname 27.01.2025.
// Реализация класса Window для создания и управления окном с использованием GLFW и GLEW.
//

#include <iostream> // Для вывода ошибок.
#include <GL/glew.h> // Для работы с OpenGL через GLEW.
#include <GLFW/glfw3.h> // Для создания окна и управления им.

#include "Window.h" // Заголовочный файл класса Window.

// Статические переменные для хранения состояния окна.
GLFWwindow* Window::window; // Указатель на окно GLFW.
int Window::width; // Ширина окна.
int Window::height; // Высота окна.

// Инициализация окна.
int Window::init(const int width, const int height, const char *title) {
    // Инициализация GLFW.
    glfwInit();

    // Настройка параметров окна.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Версия OpenGL 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Используем Core Profile.
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // Окно можно изменять.

    // Создание окна.
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate(); // Завершаем GLFW в случае ошибки.
        return -1;
    }

    // Устанавливаем текущий контекст OpenGL.
    glfwMakeContextCurrent(window);

    // Инициализация GLEW.
    glewExperimental = GL_TRUE; // Включаем экспериментальные функции.
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1; // Завершаем в случае ошибки.
    }

    // Устанавливаем область вывода OpenGL.
    glViewport(0, 0, width, height);

    // Сохраняем размеры окна.
    Window::height = height;
    Window::width = width;

    return 0; // Успешная инициализация.
}

// Завершение работы с окном.
void Window::terminate() {
    glfwDestroyWindow(window); // Уничтожаем окно.
    glfwTerminate(); // Завершаем GLFW.
}

// Установка режима курсора.
void Window::setCursorMode(const int mode) {
    glfwSetInputMode(window, GLFW_CURSOR, mode); // Устанавливаем режим (например, GLFW_CURSOR_DISABLED).
}

// Проверка, должно ли окно закрыться.
bool Window::isShouldClose() {
    return glfwWindowShouldClose(window); // Возвращает true, если окно должно закрыться.
}

// Обмен буферов (отрисовка кадра).
void Window::swapBuffers() {
    glfwSwapBuffers(window); // Обмениваем буферы для отображения кадра.
}

// Установка флага закрытия окна.
void Window::setShouldClose(const bool flag) {
    glfwSetWindowShouldClose(window, flag); // Устанавливает флаг закрытия окна.
}
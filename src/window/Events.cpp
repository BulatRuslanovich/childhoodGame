//
// Файл создан getname 27.01.2025.
// Реализация обработки событий ввода (клавиатура, мышь) с использованием GLFW.
//

#include "Events.h"
#include <cstring> // Для memset.
#include <GLFW/glfw3.h> // Для работы с GLFW.

#define MOUSE_BUTTON 1024 // Смещение для хранения событий кнопок мыши.

// Статические переменные для хранения состояния ввода.
bool* Events::_keys; // Массив для хранения состояния клавиш и кнопок мыши.
bool Events::_cursor_locked = false; // Флаг, указывающий, заблокирован ли курсор.
bool Events::_cursor_started = false; // Флаг, указывающий, начато ли отслеживание курсора.
uint* Events::_frames; // Массив для хранения кадров, в которых произошли события.
uint Events::_current = 0; // Текущий кадр.
float Events::x = 0.0f; // Текущая позиция курсора по X.
float Events::y = 0.0f; // Текущая позиция курсора по Y.
float Events::dx = 0.0f; // Изменение позиции курсора по X с последнего кадра.
float Events::dy = 0.0f; // Изменение позиции курсора по Y с последнего кадра.

// Callback-функция для обработки событий клавиатуры.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    (void)scancode;
    (void)mode;
    (void)window;
    if (action == GLFW_PRESS) {
        Events::_keys[key] = true; // Клавиша нажата.
        Events::_frames[key] = Events::_current; // Запоминаем кадр, в котором произошло событие.
    } else if (action == GLFW_RELEASE) {
        Events::_keys[key] = false; // Клавиша отпущена.
        Events::_frames[key] = Events::_current; // Запоминаем кадр, в котором произошло событие.
    }
}

// Callback-функция для обработки событий кнопок мыши.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
    (void)window;
    (void)mode;
    if (action == GLFW_PRESS) {
        Events::_keys[MOUSE_BUTTON + button] = true; // Кнопка мыши нажата.
        Events::_frames[MOUSE_BUTTON + button] = Events::_current; // Запоминаем кадр.
    } else if (action == GLFW_RELEASE) {
        Events::_keys[MOUSE_BUTTON + button] = false; // Кнопка мыши отпущена.
        Events::_frames[MOUSE_BUTTON + button] = Events::_current; // Запоминаем кадр.
    }
}

// Callback-функция для обработки перемещения курсора.
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    (void)window;
    if (Events::_cursor_started) {
        // Вычисляем изменение позиции курсора.
        Events::dx += xpos - Events::x;
        Events::dy += ypos - Events::y;
    } else {
        Events::_cursor_started = true; // Начинаем отслеживание курсора.
    }

    // Обновляем текущую позицию курсора.
    Events::x = xpos;
    Events::y = ypos;
}

// Callback-функция для обработки изменения размера окна.
void window_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height); // Обновляем область вывода OpenGL.
    Window::height = height; // Сохраняем новую высоту окна.
    Window::width = width; // Сохраняем новую ширину окна.
}

// Инициализация обработки событий.
int Events::init() {
    GLFWwindow* window = Window::window;

    // Выделяем память для хранения состояния клавиш и кадров.
    _keys = new bool[1032];
    _frames = new uint[1032];

    // Инициализируем массивы нулями.
    memset(_keys, false, sizeof(bool) * 1032);
    memset(_frames, 0, sizeof(uint) * 1032);

    // Устанавливаем callback-функции для обработки событий.
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    return 0; // Успешная инициализация.
}

// Обновление состояния событий.
void Events::pullEvents() {
    dx = 0.0f; // Сбрасываем изменение позиции курсора по X.
    dy = 0.0f; // Сбрасываем изменение позиции курсора по Y.
    _current++; // Увеличиваем счетчик кадров.
    glfwPollEvents(); // Обрабатываем события GLFW.
}

// Проверка, нажата ли клавиша.
bool Events::pressed(const int key) {
    return key < 0 || key >= MOUSE_BUTTON ? false : _keys[key];
}

// Проверка, была ли клавиша нажата в текущем кадре.
bool Events::jpressed(const int key) {
    return key < 0 || key >= MOUSE_BUTTON ? false : _keys[key] && _frames[key] == _current;
}

// Проверка, нажата ли кнопка мыши.
bool Events::clicked(const int button) {
    return _keys[MOUSE_BUTTON + button];
}

// Проверка, была ли кнопка мыши нажата в текущем кадре.
bool Events::jclicked(const int button) {
    return _keys[MOUSE_BUTTON + button] && _frames[MOUSE_BUTTON + button] == _current;
}

// Переключение режима курсора (заблокирован/разблокирован).
void Events::toogleCursor() {
    _cursor_locked = !_cursor_locked; // Переключаем состояние.
    Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); // Устанавливаем режим курсора.
}
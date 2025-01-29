//
// Файл создан getname 29.01.2025.
// Реализация класса Camera для управления камерой в 3D-сцене.
//

#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp> // Для glm::perspective.
#include <glm/ext/matrix_transform.hpp>  // Для glm::lookAt и glm::rotate.
#include "Window.h"                      // Для получения размеров окна.

// Обновляет векторы направления камеры (front, right, up) на основе текущей матрицы вращения.
void Camera::updateVectors() {
    // Вектор "вперед" (направление, куда смотрит камера).
    front = glm::vec3(rotation * glm::vec4(0, 0, -1, 0));
    // Вектор "вправо" (направление вправо относительно камеры).
    right = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
    // Вектор "вверх" (направление вверх относительно камеры).
    up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));
}

// Конструктор: инициализирует камеру с заданной позицией и углом обзора.
Camera::Camera(const glm::vec3 pos, const float fov) : pos(pos), fov(fov) {
    // Инициализируем векторы направления.
    updateVectors();
}

// Поворачивает камеру вокруг осей X, Y и Z на заданные углы.
void Camera::rotate(const float x, const float y, const float z) {
    // Вращение вокруг оси Z (крен).
    rotation = glm::rotate(rotation, z, glm::vec3(0, 0, 1));
    // Вращение вокруг оси Y (тангаж).
    rotation = glm::rotate(rotation, y, glm::vec3(0, 1, 0));
    // Вращение вокруг оси X (рыскание).
    rotation = glm::rotate(rotation, x, glm::vec3(1, 0, 0));

    // Обновляем векторы направления после вращения.
    updateVectors();
}

// Возвращает матрицу перспективной проекции для камеры.
glm::mat4 Camera::getProjection() const {
    // Вычисляем соотношение сторон окна.
    const float aspect = static_cast<float>(Window::width) / static_cast<float>(Window::height);
    // Создаем матрицу перспективной проекции.
    return glm::perspective(fov, aspect, 0.1f, 100.0f);
}

// Возвращает матрицу вида, которая определяет, как сцена будет отображаться из позиции камеры.
glm::mat4 Camera::getView() const {
    // Матрица вида строится на основе позиции камеры, направления взгляда и вектора "вверх".
    return glm::lookAt(pos, pos + front, up);
}
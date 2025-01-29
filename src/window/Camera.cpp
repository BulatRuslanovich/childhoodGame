//
// Created by getname on 29.01.2025.
//

#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Window.h"

void Camera::updateVectors() {
    // Обновляем вектор направления вперед, используя матрицу вращения.
    front = glm::vec3(rotation * glm::vec4(0, 0, -1, 0));
    // Обновляем вектор направления вправо.
    right = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
    // Обновляем вектор направления вверх.
    up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));
}

Camera::Camera(const glm::vec3 pos, const float fov) : pos(pos), fov(fov) {
    // Инициализируем позицию и угол обзора, затем обновляем векторы направления.
    updateVectors();
}

void Camera::rotate(const float x, const float y, const float z) {
    // Поворачиваем камеру вокруг оси Z на угол z.
    rotation = glm::rotate(rotation, z, glm::vec3(0, 0, 1));
    // Поворачиваем камеру вокруг оси Y на угол y.
    rotation = glm::rotate(rotation, y, glm::vec3(0, 1, 0));
    // Поворачиваем камеру вокруг оси X на угол x.
    rotation = glm::rotate(rotation, x, glm::vec3(1, 0, 0));

    // Обновляем векторы направления после изменения углов.
    updateVectors();
}

glm::mat4 Camera::getProjection() const {
    // Вычисляем соотношение сторон окна.
    const float aspect = static_cast<float>(Window::width) / static_cast<float>(Window::height);
    // Возвращаем матрицу перспективной проекции.
    return glm::perspective(fov, aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::getView() const {
    // Возвращаем матрицу вида, которая определяет, как сцена будет отображаться из позиции камеры.
    return glm::lookAt(pos, pos + front, up);
}



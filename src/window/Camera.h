//
// Created by getname on 29.01.2025.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>


class Camera {
    // Обновляет векторы направления камеры (front, up, right) на основе текущих углов поворота.
    void updateVectors();

public:
    // Вектор, указывающий вперед (direction).
    glm::vec3 front;
    // Вектор, указывающий вверх.
    glm::vec3 up;
    // Вектор, указывающий вправо.
    glm::vec3 right;

    // Позиция камеры в пространстве.
    glm::vec3 pos;
    // Угол обзора (field of view).
    float fov;
    // Матрица вращения камеры.
    glm::mat4 rotation;

    // Конструктор класса Camera, принимает позицию и угол обзора.
    Camera(glm::vec3 pos, float fov);

    // Метод для вращения камеры на заданные углы (x, y, z).
    void rotate(float x, float y, float z);

    // Возвращает матрицу проекции камеры.
    glm::mat4 getProjection() const;

    // Возвращает матрицу вида камеры.
    glm::mat4 getView() const;
};


#endif //CAMERA_H

//
// Файл создан getname 29.01.2025.
// Заголовочный файл для класса Camera, управляющего камерой в 3D-сцене.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp> // Используем GLM для работы с векторами и матрицами.

class Camera {
    void updateVectors();

public:
    // Вектор, указывающий направление взгляда камеры (вперед).
    glm::vec3 front;
    // Вектор, указывающий направление вверх относительно камеры.
    glm::vec3 up;
    // Вектор, указывающий направление вправо относительно камеры.
    glm::vec3 right;

    // Позиция камеры в мировом пространстве.
    glm::vec3 pos;
    // Угол обзора камеры (в радианах).
    float fov;
    // Матрица вращения камеры.
    glm::mat4 rotation;

    Camera(glm::vec3 pos, float fov);

    void rotate(float x, float y, float z);

    glm::mat4 getProjection() const;

    glm::mat4 getView() const;
};

#endif // CAMERA_H
#version 330 core

// Атрибуты вершин: позиция и текстурные координаты.
layout (location = 0) in vec3 v_pos;       // Позиция вершины в локальном пространстве (x, y, z).
layout (location = 1) in vec2 v_texCoord;  // Текстурные координаты вершины (UV).
layout (location = 2) in float v_light;

// Выходные данные шейдера: передаются во фрагментный шейдер.
out vec4 a_color;       // Цвет вершины (по умолчанию белый).
out vec2 a_texCoord;    // Текстурные координаты.

// Uniform-переменные: матрицы преобразований.
uniform mat4 model;     // Матрица модели (преобразует локальные координаты в мировые).
uniform mat4 projview;  // Комбинированная матрица проекции и вида (преобразует мировые координаты в экранные).

void main() {
    // Устанавливаем цвет вершины.
    a_color = vec4(v_light,v_light,v_light,1.0f);

    // Передаем текстурные координаты во фрагментный шейдер.
    a_texCoord = v_texCoord;

    // Преобразуем позицию вершины в экранные координаты.
    gl_Position = projview * model * vec4(v_pos, 1.0);
}
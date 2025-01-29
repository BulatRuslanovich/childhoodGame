#version 330 core

// Входные атрибуты вершин: цвет и текстурные координаты.
in vec4 a_color;       // Цвет вершины (RGBA).
in vec2 a_texCoord;    // Текстурные координаты (UV).

// Выходное значение фрагментного шейдера: итоговый цвет пикселя.
out vec4 f_color;

// Текстура, передаваемая через uniform.
uniform sampler2D u_texture0; // Сэмплер для текстуры.

void main() {
    // Рассчитываем итоговый цвет, умножая цвет вершины на цвет текстуры.
    f_color = a_color * texture(u_texture0, a_texCoord);
}
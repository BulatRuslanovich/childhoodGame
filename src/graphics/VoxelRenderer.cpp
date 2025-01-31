//
// Created by getname on 01.02.2025.
//

#include "VoxelRenderer.h"

#include "Mesh.h"
#include "voxels/Chunk.h"
#include "voxels/voxel.h"

// Размер данных вершины в элементах (X,Y,Z + U,V + уровень освещения)
#define VERTEX_SIZE (3 + 2 + 1)

// Проверка нахождения координат внутри чанка
#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < CHUNK_W && (Y) >= 0 && (Y) < CHUNK_H && (Z) >= 0 && (Z) < CHUNK_D)

// Получение вокселя по координатам (x,y,z)
#define VOXEL(X,Y,Z) (chunk->voxels[((Y) * CHUNK_D + (Z)) * CHUNK_W + (X)])

// Проверка существования блока в указанной позиции
#define IS_BLOCKED(X,Y,Z) ((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).id)

// Макрос для заполнения буфера вершинами
#define VERTEX(INDEX, X,Y,Z, U,V, L) buffer[INDEX+0] = (X);\
buffer[INDEX+1] = (Y);\
buffer[INDEX+2] = (Z);\
buffer[INDEX+3] = (U);\
buffer[INDEX+4] = (V);\
buffer[INDEX+5] = (L);\
INDEX += VERTEX_SIZE;

// Атрибуты вершин для меша: 3D позиция, UV-координаты, освещение
int chunk_attrs[] = {3, 2, 1, 0};

// Конструктор: выделение памяти под максимальное количество вершин
VoxelRenderer::VoxelRenderer(const size_t capacity): capacity(capacity) {
    buffer = new float[capacity * VERTEX_SIZE * 6];
}

// Деструктор: освобождение памяти
VoxelRenderer::~VoxelRenderer() {
    delete[] buffer;
}

// Ключевые особенности:
// Используется отложенная генерация только видимых граней (culling)
// Текстурный атлас 16x16
// Разные уровни освещения для граней
// Оптимизированное хранение вершин в буфере
// Поддержка чанков размером CHUNK_W x CHUNK_H x CHUNK_D
// Каждая грань генерируется только если соседний блок отсутствует или прозрачный.

// Основной метод рендеринга чанка в меш
Mesh *VoxelRenderer::render(Chunk *chunk) const {
    // Текущая позиция в буфере
    size_t index = 0;

    // Обход всех вокселей в чанке
    for (int y = 0; y < CHUNK_H; y++) {
        for (int z = 0; z < CHUNK_D; z++) {
            for (int x = 0; x < CHUNK_W; x++) {
                // Получаем ID текущего вокселя
                const auto [id] = chunk->voxels[(y * CHUNK_D + z) * CHUNK_W + x];

                // Пропускаем воздушные блоки (id = 0)
                if (!id) {
                    continue;
                }

                // Уровень освещения
                float l;

                // Размер одной текстуры в атласе
                constexpr float uvsize = 1.0f / 16.0f;

                // U-координата в атласе
                const float u = (id % 16) * uvsize;

                // V-координата (с инверсией)
                const float v = 1 - ((1 + id / 16) * uvsize);

                // Генерация граней только если соседний блок прозрачный/отсутствует

                // Верхняя грань (Y+)
                if (!IS_BLOCKED(x, y + 1, z)) {
                    // Максимальное освещение
                    l = 1.0f;

                    // Добавляем два треугольника для грани
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
                }

                // Нижняя грань (Y-)
                if (!IS_BLOCKED(x, y - 1, z)) {
                    l = 0.75f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }

                // Правая грань (X+)
                if (!IS_BLOCKED(x + 1, y, z)) {
                    l = 0.95f;
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                }

                // Левая грань (X-)
                if (!IS_BLOCKED(x - 1, y, z)) {
                    l = 0.85f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }

                // Передняя грань (Z+)
                if (!IS_BLOCKED(x, y, z + 1)) {
                    l = 0.9f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u, v+uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                }

                // Задняя грань (Z-)
                if (!IS_BLOCKED(x, y, z - 1)) {
                    l = 0.8f;
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                }
            }
        }
    }

    // Создание меша из сгенерированных данных
    return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs);
}

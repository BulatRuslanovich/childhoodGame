//
// Created by getname on 29.01.2025.
//

#include "PngLoader.h"
#include "../graphics/Texture.h"

#include <cstdio>
#include <cstdlib>
#include <png.h>
#include <string>
#include <GL/gl.h>

int _pngLoad(const char *filename, int *width, int *height) {
    // Объявление переменных
    FILE *f; // Указатель на файл
    int bit_depth, color_type; // Переменные для хранения информации о PNG
    png_infop info_ptr, end_info; // Указатели на структуры информации о PNG
    png_uint_32 t_width, t_height; // Ширина и высота изображения
    png_byte header[8]; // Заголовок PNG и указатель на данные изображения
    png_structp png_ptr; // Указатель на структуру чтения PNG
    GLuint texture; // Идентификатор текстуры OpenGL
    int alpha; // Переменная для хранения формата цвета

    // Открытие файла для чтения
    if (!((f = fopen(filename, "r")))) {
        return 0; // Возврат 0, если файл не удалось открыть
    }

    // Чтение первых 8 байтов файла (заголовок PNG)
    fread(header, 1, 8, f);

    // Если файл не PNG, закрыть его и вернуть 0
    if (const int is_png = !png_sig_cmp(header, 0, 8); !is_png) {
        fclose(f);
        return 0;
    }

    // Создание структуры для чтения PNG
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    // Проверка на успешное создание структуры
    if (!png_ptr) {
        fclose(f);
        return 0; // Возврат 0 в случае ошибки
    }

    // Создание структуры информации о PNG
    info_ptr = png_create_info_struct(png_ptr);

    // Проверка на успешное создание структуры
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(f);
        return 0; // Возврат 0 в случае ошибки
    }

    // Создание структуры для конечной информации о PNG
    end_info = png_create_info_struct(png_ptr);

    // Проверка на успешное создание структуры
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(f);
        return 0; // Возврат 0 в случае ошибки
    }

    // Установка точки возврата для обработки ошибок
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        return 0; // Возврат 0 в случае ошибки
    }

    // Инициализация ввода из файла
    png_init_io(png_ptr, f);
    // Установка количества байтов в заголовке
    png_set_sig_bytes(png_ptr, 8);
    // Чтение информации об изображении
    png_read_info(png_ptr, info_ptr);
    // Получение информации о заголовке изображения (ширина, высота, глубина и тип цвета)
    png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth, &color_type, nullptr, nullptr, nullptr);
    // Запись ширины и высоты в переданные указатели
    *width = t_width;
    *height = t_height;
    // Обновление информации об изображении
    png_read_update_info(png_ptr, info_ptr);
    // Получение количества байтов в строке изображения
    const int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    // Выделение памяти для данных изображения
    const auto image_data = static_cast<png_bytep>(malloc(row_bytes * t_height * sizeof(png_bytep)));  // Указатель на строки изображения

    // Проверка на успешное выделение памяти
    if (!image_data) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        return 0; // Возврат 0 в случае ошибки
    }

    // Выделение памяти для указателя на строки изображения
    const auto row_pointer = static_cast<png_bytepp>(malloc(t_height * sizeof(png_bytepp)));

    // Проверка на успешное выделение памяти
    if (!row_pointer) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(f);
        return 0; // Возврат 0 в случае ошибки
    }

    // Заполнение указателя на строки данными изображения
    for (unsigned int i = 0; i < t_height; ++i) {
        row_pointer[t_height - 1 - i] = image_data + i * row_bytes;
    }

    // Чтение изображения из файла
    png_read_image(png_ptr, row_pointer);

    // Определение типа цвета и установка соответствующего формата
    switch (png_get_color_type(png_ptr, info_ptr)) {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA; // Установка формата RGBA
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB; // Установка формата RGB
            break;
        default:
            printf("unsupported color type\n"); // Сообщение об ошибке для неподдерживаемого типа цвета
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            free(image_data);
            free(row_pointer);
            fclose(f);
            return 0; // Возврат 0 в случае ошибки
    }

    // Генерация текстуры OpenGL
    glGenTextures(1, &texture);
    // Привязка текстуры
    glBindTexture(GL_TEXTURE_2D, texture);
    // Установка выравнивания пикселей
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Загрузка данных изображения в текстуру
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, alpha, GL_UNSIGNED_BYTE, image_data);
    // Установка параметров фильтрации текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Отвязка текстуры
    glBindTexture(GL_TEXTURE_2D, 0);

    // Освобождение ресурсов
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data); // Освобождение памяти для данных изображения
    free(row_pointer); // Освобождение памяти для указателя на строки
    fclose(f); // Закрытие файла
    return texture; // Возврат идентификатора текстуры
}


Texture* loadTexture(const std::string &filename) {
    int width, height;
    const GLint texture = _pngLoad(filename.c_str(), &width, &height);

    if (!texture) {
        printf("Failed to load texture %s\n", filename.c_str());
        return nullptr;
    }

    return new Texture(texture, width, height);
}

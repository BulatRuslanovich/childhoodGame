//
// Файл создан getname 29.01.2025.
// Реализация загрузки PNG-текстур с использованием библиотеки libpng.
//

#include "PngLoader.h"
#include "../graphics/Texture.h"

#include <cstdio>
#include <cstdlib>
#include <png.h>
#include <string>
#include <GL/gl.h>

// Внутренняя функция для загрузки PNG-файла и создания текстуры OpenGL.
int _pngLoad(const char *filename, int *width, int *height) {
    FILE *f; // Указатель на файл.
    int bit_depth, color_type; // Глубина цвета и тип цвета PNG.
    png_infop info_ptr, end_info; // Структуры для хранения информации о PNG.
    png_uint_32 t_width, t_height; // Ширина и высота изображения.
    png_byte header[8]; // Заголовок PNG (первые 8 байт).
    png_structp png_ptr; // Структура для чтения PNG.
    GLuint texture; // Идентификатор текстуры OpenGL.
    int alpha; // Формат цвета (GL_RGBA или GL_RGB).

    // Открываем файл для чтения.
    if (!(f = fopen(filename, "rb"))) {
        printf("Failed to open file: %s\n", filename);
        return 0; // Возвращаем 0, если файл не удалось открыть.
    }

    // Читаем заголовок PNG (первые 8 байт).
    fread(header, 1, 8, f);

    // Проверяем, является ли файл PNG.
    if (png_sig_cmp(header, 0, 8)) {
        fclose(f);
        printf("File is not a valid PNG: %s\n", filename);
        return 0; // Возвращаем 0, если файл не PNG.
    }

    // Создаем структуру для чтения PNG.
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(f);
        printf("Failed to create PNG read struct.\n");
        return 0; // Возвращаем 0 в случае ошибки.
    }

    // Создаем структуру для хранения информации о PNG.
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(f);
        printf("Failed to create PNG info struct.\n");
        return 0; // Возвращаем 0 в случае ошибки.
    }

    // Создаем структуру для хранения конечной информации о PNG.
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(f);
        printf("Failed to create PNG end info struct.\n");
        return 0; // Возвращаем 0 в случае ошибки.
    }

    // Настраиваем обработку ошибок через setjmp.
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        printf("Error during PNG reading.\n");
        return 0; // Возвращаем 0 в случае ошибки.
    }

    // Инициализируем ввод из файла.
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8); // Указываем, что заголовок уже прочитан.

    // Читаем информацию о PNG.
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth, &color_type, nullptr, nullptr, nullptr);

    // Сохраняем ширину и высоту.
    *width = t_width;
    *height = t_height;

    // Обновляем информацию о PNG для корректного чтения данных.
    png_read_update_info(png_ptr, info_ptr);

    // Вычисляем количество байтов в строке изображения.
    const int row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    // Выделяем память для данных изображения.
    png_bytep image_data = static_cast<png_bytep>(malloc(row_bytes * t_height));
    if (!image_data) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        printf("Failed to allocate memory for image data.\n");
        return 0; // Возвращаем 0 в случае ошибки.
    }

    // Выделяем память для указателей на строки.
    png_bytepp row_pointer = static_cast<png_bytepp>(malloc(t_height * sizeof(png_bytep)));
    if (!row_pointer) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(f);
        printf("Failed to allocate memory for row pointers.\n");
        return 0; // Возвращаем 0 в случае ошибки.
    }

    // Заполняем указатели на строки (в обратном порядке для OpenGL).
    for (unsigned int i = 0; i < t_height; ++i) {
        row_pointer[t_height - 1 - i] = image_data + i * row_bytes;
    }

    // Читаем изображение.
    png_read_image(png_ptr, row_pointer);

    // Определяем формат цвета (RGBA или RGB).
    switch (color_type) {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB;
            break;
        default:
            printf("Unsupported color type: %d\n", color_type);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            free(image_data);
            free(row_pointer);
            fclose(f);
            return 0; // Возвращаем 0 для неподдерживаемых форматов.
    }

    // Генерируем текстуру OpenGL.
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Устанавливаем выравнивание пикселей.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, alpha, GL_UNSIGNED_BYTE, image_data);

    // Устанавливаем параметры фильтрации текстуры.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Отвязываем текстуру.
    glBindTexture(GL_TEXTURE_2D, 0);

    // Освобождаем ресурсы.
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointer);
    fclose(f);

    return texture; // Возвращаем идентификатор текстуры.
}

// Функция для загрузки текстуры из файла.
Texture* loadTexture(const std::string &filename) {
    int width, height;
    const GLint texture = _pngLoad(filename.c_str(), &width, &height);

    if (!texture) {
        printf("Failed to load texture: %s\n", filename.c_str());
        return nullptr; // Возвращаем nullptr в случае ошибки.
    }

    return new Texture(texture, width, height); // Возвращаем объект Texture.
}
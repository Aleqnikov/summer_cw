#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    unsigned short signature;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixelArrOffset;
} BitmapFileHeader;

typedef struct {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BitmapInfoHeader;

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Rgb;

#pragma pack(pop)

// Функция для считывания BMP файла в двумерный массив Rgb
int read_bmp(const char *input_file, BitmapFileHeader *file_header, BitmapInfoHeader *info_header, Rgb ***data) {
    FILE *file = fopen(input_file, "rb");

    fread(file_header, sizeof(BitmapFileHeader), 1, file);

    fread(info_header, sizeof(BitmapInfoHeader), 1, file);

    fseek(file, file_header->pixelArrOffset, SEEK_SET);
    int row_padded = (info_header->width * 3 + 3) & (~3);
    *data = (Rgb **)malloc(abs(info_header->height) * sizeof(Rgb *));


    for (int i = 0; i < abs(info_header->height); i++) {
        (*data)[i] = (Rgb *)malloc(info_header->width * sizeof(Rgb));
    }

    // Чтение данных пикселей в двумерный массив Rgb с учетом порядка строк
    for (int y = 0; y < abs(info_header->height); y++) {
        for (int x = 0; x < info_header->width; x++) {
            fread(&(*data)[y][x], sizeof(Rgb), 1, file);
        }

        fseek(file, row_padded - info_header->width * 3, SEEK_CUR);
    }

    fclose(file);
    return 1;
}

// Функция для инвертирования цветов в двумерном массиве
void invert_colors(Rgb **data, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            data[y][x].r = data[y][x].g;
            data[y][x].g = data[y][x].b;
            data[y][x].b = data[y][x].r;
        }
    }
}

// Функция для рисования круга в двумерном массиве
void draw_circle(Rgb **data, int width, int height, int cx, int cy, int r, Rgb color) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = x - cx;
            int dy = y - cy;
            if (dx * dx + dy * dy + 1200 <= r * r && dx * dx + dy * dy <= r * r) {
                data[y][x] = color;
            }
        }
    }
}


// Функция для записи BMP файла из двумерного массива
int write_bmp(const char *output_file, const BitmapFileHeader *file_header, const BitmapInfoHeader *info_header, Rgb **data) {
    FILE *file = fopen(output_file, "wb");


    // Записываем заголовки
    fwrite(file_header, sizeof(BitmapFileHeader), 1, file);
    fwrite(info_header, sizeof(BitmapInfoHeader), 1, file);

    fseek(file, file_header->pixelArrOffset, SEEK_SET);

    // Рассчитываем выравнивание строк
    int row_padded = (info_header->width * 3 + 3) & (~3);

    // Записываем данные пикселей с учетом порядка строк
    for (int y = 0; y < abs(info_header->height); y++) {
        for (int x = 0; x < info_header->width; x++) {
            fwrite(&data[y][x], sizeof(Rgb), 1, file);
        }
        // Добавляем padding для выравнивания строки до кратности 4 байтам
        unsigned char padding = 0;
        for (int i = 0; i < row_padded - info_header->width * 3; i++) {
            fwrite(&padding, sizeof(unsigned char), 1, file);
        }
    }

    fclose(file);
    return 1;
}

int main() {
    char input_file[256] = "./picture1.bmp";
    char output_file[256] = "output.bmp";

    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;
    Rgb **data = NULL;

    if (!read_bmp(input_file, &file_header, &info_header, &data)) {
        return 1;
    }

    // Устанавливаем параметры круга: центр и радиус
    int circle_center_x = info_header.width / 2;
    int circle_center_y = info_header.height / 2;
    int circle_radius = 100;

    // Цвет круга (например, красный)
    Rgb circle_color = {252, 52, 52};  // RGB: красный

    // Рисуем круг
    draw_circle(data, info_header.width, info_header.height, circle_center_x, circle_center_y, circle_radius, circle_color);

    // Инвертируем цвета
    invert_colors(data, info_header.width, info_header.height);

    // Записываем измененное изображение
    if (!write_bmp(output_file, &file_header, &info_header, data)) {
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < abs(info_header.height); i++) {
        free(data[i]);
    }
    free(data);

    printf("Processing complete. Saved as %s\n", output_file);
    return 0;
}

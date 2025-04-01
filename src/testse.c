#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

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



int custom_cos(int angle) {
    switch (angle) {
        case 90:
            return 0;
        case 180:
            return -1;
        case 270:
            return 0;
    }
}

int custom_sin(int angle) {
    switch (angle) {
        case 90:
            return 1;
        case 180:
            return 0;
        case 270:
            return -1;
    }
}


// Функция для считывания BMP файла в двумерный массив Rgb
int read_bmp(const char *input_file, BitmapFileHeader *file_header, BitmapInfoHeader *info_header, Rgb ***data) {
    FILE *file = fopen(input_file, "rb");

    fread(file_header, sizeof(BitmapFileHeader), 1, file);

    fread(info_header, sizeof(BitmapInfoHeader), 1, file);

    fseek(file, file_header->pixelArrOffset, SEEK_SET);
    int row_padded = (info_header->width * 3 + 3) & (~3);
    *data = (Rgb** )(malloc(sizeof(Rgb* )* (info_header->height)));

    for (int i = 0; i < (info_header->height); i++) {
        (*data)[i] = (Rgb *)malloc(info_header->width * sizeof(Rgb));
    }
    // Чтение данных пикселей в двумерный массив Rgb с учетом порядка строк
    for (int y = 0; y < (info_header->height); y++) {
        for (int x = 0; x < info_header->width; x++) {
            fread(&(*data)[y][x], sizeof(Rgb), 1, file);
        }

        fseek(file, row_padded - info_header->width * 3, SEEK_CUR);
    }

    fclose(file);
    return 1;
}




typedef struct Object {
    /** @brief Режим работы программы.*/

    char* start_filename;                               /** @brief Имя файла, который нужно обработать.*/
    char* finish_filename;                              /** @brief Имя файла, который должно быть после обработки.*/

    int x_left_up, y_left_up;                           /** @brief Координаты левой верхней точки.*/
    int x_right_down, y_right_down;                     /** @brief Координаты правой нижней точки.*/

    int thinckness;                                     /** @brief Толшина линии.*/

    int color_r, color_g, color_b;                      /** @brief Цвет линии.*/

    bool fill;                                          /** @brief Нужно ли заливать фигуру.*/

    int color_fill_r, color_fill_g, color_fill_b;       /** @brief Цвета заливки*/


    int count;                                          /** @brief Количество элементов узора.*/

    int angle;                                          /** @brief Угол поворота части изображения.*/

    int x_center, y_center;                             /** @brief Координаты центра окружности.*/
    int radius;                                         /** @brief Радиус окружности*/
} object_t ;





Rgb** copy_array(Rgb** data, int height, int width){
    Rgb** new_array = NULL;
    new_array = (Rgb** )(malloc(sizeof(Rgb* )* (height)));

    for (int i = 0; i < (height); i++) {
        (new_array)[i] = (Rgb *)malloc(width * sizeof(Rgb));
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            new_array[i][j] = data[i][j];
        }
    }

    return new_array;
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
    for (int y = 0; y < (info_header->height); y++) {
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
    char input_file[256] = "./src/dataset/picture1.bmp";
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
    Rgb circle_color = {0, 0, 255};  // RGB: красный


    object_t info_m;
    info_m.angle = 90;
    info_m.x_left_up = 100;
    info_m.x_right_down = 400;
    info_m.y_right_down = 100;
    info_m.y_left_up = 110;

    int x_center = (info_m.x_left_up + info_m.x_right_down)/2;
    int y_center = (info_m.y_left_up + info_m.y_right_down)/2;
    Rgb** new_data = copy_array(data, info_header.height, info_header.width);

    for (int y = info_m.y_right_down; y <= info_m.y_left_up; y++) {
        for (int x = info_m.x_left_up; x <= info_m.x_right_down; x++) {
            // Смещаем пиксель относительно центра
            int x_shifted = x - x_center;
            int y_shifted = y - y_center;

            // Поворот
            int x_new = (int)((x_shifted * custom_cos(info_m.angle) + y_shifted * custom_sin(info_m.angle)) + x_center);
            int y_new = (int)((-x_shifted *custom_sin(info_m.angle)) + y_shifted * custom_cos(info_m.angle) + y_center);
            
            new_data[y_new][x_new] = data[y][x];

        }
    }




    // Записываем измененное изображение
    if (!write_bmp(output_file, &file_header, &info_header, new_data)) {
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < (info_header.height); i++) {
        free(data[i]);
    }
    free(data);

    printf("Processing complete. Saved as %s\n", output_file);
    return 0;
}

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

// Функция для инвертирования цветов в двумерном массиве
void invert_colors(Rgb **data, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            data[y][x].r = 255 - data[y][x].r;
            data[y][x].g = 255 - data[y][x].g;
            data[y][x].b = 255 - data[y][x].b;
        }
    }
}

// Функция для рисования круга в двумерном массиве
void draw_circle(Rgb **data, int width, int height, int cx, int cy, int r, Rgb color) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = x - cx;
            int dy = y - cy;
            if (dx * dx + dy * dy - 300 <= r * r && dx * dx + dy * dy > r * r) {
                data[y][x] = color; 
            }
        }
    }
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

bool check_draw_thincless(int y, int x, int thincless, int x_min, int x_max, int y_min, int y_max){
    if(y >= y_min - thincless && y <= y_max + thincless && x >= x_min - thincless && x < x_min)
        return 1;
    
    if(y >= y_min - thincless && y <= y_max + thincless && x > x_max  && x <= x_max + thincless)
        return 1;

    if(x >= x_min - thincless && x <= x_max + thincless && y >= y_min - thincless && y < y_min)
        return 1;
    
    if(x >= x_min - thincless && x <= x_max + thincless && y > y_max  && y <= y_max + thincless)
        return 1;

    return 0;
}


void draw_rectangle(Rgb **data){

    object_t info_m;
    info_m.fill = true;
    info_m.x_left_up = 10;
    info_m.x_right_down = 20;
    info_m.y_right_down = 10;
    info_m.y_left_up = 20;



    info_m.thinckness = 1;

    Rgb color = {14, 34, 52};

    data[200][200] = color;

    Rgb color_thinckless = {4, 244, 34};

    for(int y = info_m.y_right_down - info_m.thinckness; y <= info_m.y_left_up + info_m.thinckness; y++){
        for(int x = info_m.x_left_up - info_m.thinckness; x <= info_m.x_right_down + info_m.thinckness; x++){

            if(check_draw_thincless(y, x, info_m.thinckness, info_m.x_left_up, info_m.x_right_down, info_m.y_right_down, info_m.y_left_up))
                    data[y][x] = color_thinckless;

            if(info_m.fill){
                if(x <= info_m.x_right_down && x >= info_m.x_left_up && y >= info_m.y_right_down && y <= info_m.y_left_up)
                    data[y][x] = color;
            }

        }
    }
}

void draw_rectangle2(Rgb **data){

    object_t info_m;
    info_m.fill = false;
    info_m.x_left_up = 100;
    info_m.x_right_down = 300;
    info_m.y_right_down = 50;
    info_m.y_left_up = 300;



    info_m.thinckness = 1;

    Rgb color = {14, 34, 52};

    data[200][200] = color;

    Rgb color_thinckless = {4, 244, 34};

    if(info_m.fill){

        for(int y = info_m.y_right_down - info_m.thinckness; y < info_m.y_left_up + info_m.thinckness; y++){
            for(int x = info_m.x_left_up - info_m.thinckness; x < info_m.x_right_down + info_m.thinckness; x++){
                
                data[y][x] = color;
            }
        }
    } else{
        
        // Рисуем боковые стороны прямоугольника. 
        for(int y = info_m.y_right_down - info_m.thinckness; y < info_m.y_left_up + info_m.thinckness; y++){
            for(int x = info_m.x_left_up - info_m.thinckness; x < info_m.x_left_up ; x++){
                
                    data[y][x] = color;
            }
            for(int x = info_m.x_right_down; x < info_m.x_right_down + info_m.thinckness; x++){
                
                    data[y][x] = color;
            }
        }

        // Рисуем верхние стороны прямоугольника. 
        for(int x = info_m.x_left_up - info_m.thinckness; x < info_m.x_right_down + info_m.thinckness; x++){
            for(int y = info_m.y_right_down - info_m.thinckness ; y < info_m.x_left_up ; y++){
              
                    data[y][x] = color;
            }
            for(int y = info_m.y_left_up; y < info_m.y_left_up + info_m.thinckness; y++){
                
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
    char input_file[256] = "./dataset/picture1.bmp";
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

    // Рисуем круг
    draw_circle(data, info_header.width, info_header.height, circle_center_x, circle_center_y, circle_radius, circle_color);

    draw_rectangle(data);
    // Инвертируем цвета
    invert_colors(data, info_header.width, info_header.height);

    // Записываем измененное изображение
    if (!write_bmp(output_file, &file_header, &info_header, data)) {
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

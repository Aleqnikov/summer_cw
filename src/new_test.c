#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <math.h>


/**
 * @file main.с
 * @brief Весенняя курсовая работа
 */


/**
 * @brief Перечисление режимов для орнамента.
 *
 * В данном перечислении находятся возможные режимы для создания орнамента, а также режим none, которые говорит о том,
 * что поле структуры не инициализировано.
 */
typedef enum Pattern {rectangle, circle, semicircle, none} pattern_t;

/**
 * @brief Перечисление режимов допустимых операций.
 *
 * В данном перечислении находятся возможные режимы работы парсера, используются при инициализации полей структур,
 * а также при ветвеленни, где происходит проверка поля на соответствие того или иного режима.
 */
typedef enum Mode {rect, ornament, rotate, circ, info, help, None} mode;

typedef enum {
    base_coords_val = INT_MIN,
    base_color_val = -1,
    base_thinckness_val = -1,
    base_radius_val = -1,
    base_count_val = -1,
    base_angle_val = -1,
} initial_values;

typedef enum  {up = 1, down = -1} limit_circle_mod;


#pragma pack(push, 1)
/**
* @breif Набор структур для чтения BMP
*/
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


/**
 * @brief Структура, которая является собирательным образом всех параметров, которые нужны для
 * работы каждого из режимов. Предполагается, что взаимодействие внутри программы происходит
 * за счёт последовательной передачи объекта данной структуры.
 *
 * Данная структура включает все возможные поля для реализации всех режимов работы программы. Предполагается, что
 * Взаимодействие частей программы происходит при передачи экземпляра данной структуры внутри программы. По сути,
 * является классом данных, для более удобного формата хранения и взаимодействия с данными.
 */

typedef struct Object {
    /** @brief Режим работы программы.*/
    mode mod;

    char* start_filename;                               /** @brief Имя файла, который нужно обработать.*/
    char* finish_filename;                              /** @brief Имя файла, который должно быть после обработки.*/

    int x_left_up, y_left_up;                           /** @brief Координаты левой верхней точки.*/
    int x_right_down, y_right_down;                     /** @brief Координаты правой нижней точки.*/

    int thinckness;                                     /** @brief Толшина линии.*/

    int color_r, color_g, color_b;                      /** @brief Цвет линии.*/

    bool fill;                                          /** @brief Нужно ли заливать фигуру.*/

    int color_fill_r, color_fill_g, color_fill_b;       /** @brief Цвета заливки*/

    pattern_t pattern;                                  /** @brief Режим для создания узора.*/

    int count;                                          /** @brief Количество элементов узора.*/

    int angle;                                          /** @brief Угол поворота части изображения.*/

    int x_center, y_center;                             /** @brief Координаты центра окружности.*/
    int radius;                                         /** @brief Радиус окружности*/
} object_t ;

/**
 * @brief Используется при получении координат растровых фигут.
 */
typedef struct {
    int x, y;
} point_t;

typedef struct {
    int x_center, y_center;
    int radius;
    int thickness;
    int len_array;
    point_t* points;
} circle_t;




/*
___________________________IMAGE_TOOLS_________________________________________________________
 */


/**
 * @brief Функция которая печатает информацию headera файла.
 */
void print_file_header(BitmapFileHeader bmfh){
    printf("signature: \t%x (%hu)\n", bmfh.signature, bmfh.signature);
    printf("filesize: \t%x (%u)\n", bmfh.filesize, bmfh.filesize);
    printf("reserved1: \t%x (%hu)\n", bmfh.reserved1, bmfh.reserved1);
    printf("reserved2: \t%x (%hu)\n", bmfh.reserved2, bmfh.reserved2);
    printf("pixelArrOffset: \t%x (%u)\n", bmfh.pixelArrOffset, bmfh.pixelArrOffset);
}

/**
 * @brief Функция которая печатает информацию headera информации.
 */
void print_info_header(BitmapInfoHeader bmih){
    printf("headerSize: \t%x (%u)\n", bmih.headerSize, bmih.headerSize);
    printf("width: \t%x (%u)\n", bmih.width, bmih.width);
    printf("height: \t%x (%u)\n", bmih.height, bmih.height);
    printf("planes: \t%x (%hu)\n", bmih.planes, bmih.planes);
    printf("bitsPerPixel: \t%x (%hu)\n", bmih.bitsPerPixel, bmih.bitsPerPixel);
    printf("compression: \t%x (%u)\n", bmih.compression, bmih.compression);
    printf("imageSize: \t%x (%u)\n", bmih.imageSize, bmih.imageSize);
    printf("xpixelsPerMeter: \t%x (%u)\n", bmih.xPixelsPerMeter, bmih.xPixelsPerMeter);
    printf("ypixelsPerMeter: \t%x (%u)\n", bmih.yPixelsPerMeter, bmih.yPixelsPerMeter);
    printf("colorsInColorTable: \t%x (%u)\n", bmih.colorsInColorTable, bmih.colorsInColorTable);
    printf("importantColorCount: \t%x (%u)\n", bmih.importantColorCount, bmih.importantColorCount);
}


int get_padding(unsigned int width){
    return ((width*3 + 3) & (~3));
}

/**
 * @brief Данная функция проверяет, выходит ли за пределы изображения координаты.
 */
bool check_coord(int y, int x, int height, int weight){
    if((x >= 0 && x < weight) && (y >= 0 && y < height))
        return 1;
    return 0;
}

/**
* @brief Данная функция считывает изображение BMP
*/
int read_bmp(const char* filename, BitmapFileHeader* file_header, BitmapInfoHeader* info_header, Rgb*** data){
    FILE* file = fopen(filename, "rb");


    if(!file){
        fprintf(stderr, "Error: Не удалось открыть файл!\n");
        return 0;
    }

    fread(file_header, sizeof(BitmapFileHeader), 1, file);
    fread(info_header, sizeof(BitmapInfoHeader), 1, file);

    if(file_header->signature != 0x4D42){
        fprintf(stderr, "Error: Файл не является bmp.\n");
        return 0;
    }

    if(info_header->bitsPerPixel != 24){
        fprintf(stderr, "Error: Программа поддерживает только 24 бита на пиксель!\n");
        return 0;
    }

    if (info_header->headerSize  != 124) {
        fprintf(stderr, "Error: Поддерживается только версия BMPv5!\n");
        return 0;
    }

    if(info_header->compression != 0){
        fprintf(stderr, "Error: Программа не обрабатывает изображения со сжатием!\n");
        return 0;
    }

    fseek(file, file_header->pixelArrOffset, SEEK_SET);
    int row_padded = get_padding(info_header->width);

    // Выделяем память для массива указателей на строки пикселей
    *data = (Rgb** )(malloc(sizeof(Rgb* )* abs(info_header->height)));

    if(!*data){
        fprintf(stderr, "Error: Не удалось выделить память под массив пикселей!\n");
        return 0;
    }

    for (int i = 0; i < abs(info_header->height); i++){


        (*data)[i] = (Rgb *)malloc(info_header->width * sizeof(Rgb));

        if(!(*data)[i]){
            fprintf(stderr, "Error: Не удалось выделить память под строку пикселей!.\n");
            return 0;
        }
    }

    for(int i = 0; i < abs(info_header->height); i++){
        for(int j = 0; j < info_header->width; j++){
            fread(&(*data)[i][j], sizeof(Rgb), 1, file);
        }

        fseek(file, row_padded - info_header->width*3, SEEK_CUR);
    }

    fclose(file);

   return 1;

}

/**
 * @brief Данная функция записывает в файл.
 */
int write_bmp(const char* filename, const BitmapFileHeader* bmfh, const BitmapInfoHeader* bmih, Rgb** data){
    FILE* file = fopen(filename, "wb");

    if(!file){
        fprintf(stderr, "Error: Не удалось открыть файл для записи выходного изображения.\n");
        return 1;
    }

    fwrite(bmfh, sizeof(BitmapFileHeader), 1, file);
    fwrite(bmih, sizeof(BitmapInfoHeader), 1, file);

    fseek(file, bmfh->pixelArrOffset, SEEK_SET);

    int row_paded = get_padding(bmih->width);

    for(int y = 0; y < abs(bmih->height); y++){
        for(int x = 0; x < bmih->width; x++){
            fwrite(&data[y][x], sizeof(Rgb), 1, file);
        }

        // Добавляеся padding для выравнивания строки кратности 4.
        unsigned char padding = 0;
        for(int i = 0; i < row_paded - bmih->width*3; i++)
            fwrite(&padding, sizeof(unsigned char), 1, file);
    }

    fclose(file);
    return 0;
}


void draw_rectangle(Rgb*** data, BitmapInfoHeader bmih, object_t figure, Rgb color_fill, Rgb color) {

    int thickness = figure.thinckness;
    int x_lu = figure.x_left_up; int x_rd = figure.x_right_down;
    int y_lu = figure.y_left_up; int y_rd = figure.y_right_down;

    if (figure.fill) {
        for(int y = y_rd; y <= y_lu ; y++) {
            for(int x = x_lu; x <= x_rd ; x++)
                if(check_coord(y, x, bmih.height, bmih.width)) (*data)[y][x] = color_fill;
        }
    }

    for (int upper = - (thickness) / 2; upper <= (thickness) / 2; upper++) {
        for (int side = - (thickness) / 2; side <= (thickness) / 2; side++) {
            for(int y = y_rd; y <= y_lu ; y++) {
                if(check_coord(y + upper, x_lu + side, bmih.height, bmih.width)) (*data)[y + upper][x_lu + side] = color;
                if(check_coord(y + upper, x_rd + side, bmih.height, bmih.width)) (*data)[y + upper][x_rd + side] = color;
            }

            for(int x = x_lu; x <= x_rd ; x++) {
                if(check_coord(y_lu + upper, x + side, bmih.height, bmih.width)) (*data)[y_lu + upper][x + side] = color;
                if(check_coord(y_rd + upper, x + side, bmih.height, bmih.width)) (*data)[y_rd + upper][x + side] = color;
            }
        }
    }
}

Rgb** copy_array(Rgb** data, int height, int width){
    Rgb** new_array = NULL;
    new_array = (Rgb** )(malloc(sizeof(Rgb* ) * (height)));

    if (new_array == NULL)
        return NULL;

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

/**
 * @brief Переворачивает часть изображения
 *
 * При помощи преобразований плоскости, мы меняем координаты точек, и затем ставим её на новое место.
 * Т.е x = xcosa + ysina, y = -xsina + ycosa
 */
Rgb** rotate_area(Rgb*** data, BitmapInfoHeader bmih, object_t figure){

    Rgb** new_data = copy_array(*data, bmih.height, bmih.width);
    if (new_data == NULL)
        return NULL;

    int x_center = (figure.x_left_up + figure.x_right_down)/2;
    int y_center = (figure.y_left_up + figure.y_right_down)/2;

    for (int y = figure.y_right_down; y <= figure.y_left_up; y++) {
        for (int x = figure.x_left_up; x <= figure.x_right_down; x++) {

            int x_shifted = x - x_center;
            int y_shifted = y - y_center;

            int x_new = (int)((x_shifted * custom_cos(figure.angle) + y_shifted * custom_sin(figure.angle)) + x_center);
            int y_new = (int)((-x_shifted *custom_sin(figure.angle)) + y_shifted * custom_cos(figure.angle) + y_center);

            if(check_coord(x_new, y_new, bmih.height, bmih.width))  new_data[y_new][x_new] = (*data)[y][x];

        }
    }

    for (int i = 0; i < (bmih.height); i++)
        free((*data)[i]);
    free(*data);

    return new_data;
}

// Function implementing Bresenham's circle algorithm for the entire first quarter of the circle
int circ_bre(int rad, point_t* circle){
    int x = 0;
    int y = rad;
    int d = 3 - 2 * rad;

    int index = 0;

    while (x <= y)
    {
        circle[index].x = x;
        circle[index].y = y;

        index++;

        if (x != y){
            circle[index].x = y;
            circle[index].y = x;
            index++;
        }

        if (d < 0) {
            d = d + 4 * x + 6;
        }
        else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    return index;
}

int get_y_limit(circle_t cirlce, int x_val, limit_circle_mod mode){
    int y = (mode == down? INT_MAX: INT_MIN);

    for (int i = 0; i < cirlce.len_array; i++) {

        if ((cirlce.points[i]).x + cirlce.x_center == x_val) {
            if (mode == down)
                y = y < (cirlce.points[i]  ).y ? y : (cirlce.points[i]).y;
            else
                y = y > (cirlce.points[i]  ).y ? y : (cirlce.points[i]).y;
        }
    }

    if (y == (mode == down ? INT_MAX: INT_MIN))
        return 0;
    return y;
}

int draw_thicnless(Rgb*** data, circle_t big_circle, circle_t min_circle, BitmapInfoHeader bmih, Rgb color) {
    int x_min = big_circle.x_center; int x_max = big_circle.radius + big_circle.x_center;
    int y_min = big_circle.y_center;

    for (int j = x_min; j <= x_max; j++) {
        int y_up_lim = get_y_limit(big_circle, j, up);
        int y_down_lim = get_y_limit(min_circle, j, down);

        for (int i = y_down_lim; i <= y_up_lim; i++) {
            if (check_coord(y_min + i, j, bmih.height, bmih.width))
                (*data)[y_min + i][j] = color;
            if (check_coord(y_min - i, j, bmih.height, bmih.width))
                (*data)[y_min - i][j] = color;
            if (check_coord(y_min + i, 2*x_min - j, bmih.height, bmih.width))
                (*data)[y_min + i][2*x_min - j] = color;
            if (check_coord(y_min - i, 2* x_min - j, bmih.height, bmih.width))
                (*data)[y_min - i][2* x_min - j] = color;
        }
    }
    return 1;
}

int draw_fill_circle(Rgb*** data, BitmapInfoHeader bmih, circle_t circle , Rgb color) {
    int x_min = circle.x_center; int x_max = circle.radius + circle.x_center ;
    int y_min = circle.y_center;


    for (int j = x_min; j <= x_max; j++) {
        int y_up_lim = get_y_limit(circle, j, up);

        for (int i = 0; i <= y_up_lim; i++) {
            if (check_coord(y_min + i, j, bmih.height, bmih.width))
                (*data)[y_min + i][j] = color;
            if (check_coord(y_min - i, j, bmih.height, bmih.width))
                (*data)[y_min - i][j] = color;
            if (check_coord(y_min + i, 2*x_min - j, bmih.height, bmih.width))
                (*data)[y_min + i][2*x_min - j] = color;
            if (check_coord(y_min - i, 2* x_min - j, bmih.height, bmih.width))
                (*data)[y_min - i][2* x_min - j] = color;
        }
    }

    return 1;
}

int draw_circle(Rgb*** data, BitmapInfoHeader bmih, Rgb color, Rgb color_fill, object_t figure) {
    figure.thinckness = (figure.thinckness  % 2 == 0) ? figure.thinckness + 1 : figure.thinckness;

    int radius_min = figure.radius - figure.thinckness / 2;
    int radius_max = figure.radius + figure.thinckness / 2;

    circle_t min_circle = {
        .points = malloc(sizeof(point_t) * radius_min * 8),
        .radius = radius_min,
        .x_center = figure.x_center,
        .y_center = figure.y_center,
        .thickness = figure.thinckness,
    };

    circle_t big_circle = {
        .points = malloc(sizeof(point_t) * radius_max * 8),
        .radius = radius_max,
        .x_center = figure.x_center,
        .y_center = figure.y_center,
        .thickness = figure.thinckness,
    };

    min_circle.len_array = circ_bre(radius_min, min_circle.points);
    big_circle.len_array = circ_bre(radius_max, big_circle.points);

    if (figure.fill)
        draw_fill_circle(data, bmih, min_circle, color_fill);

    draw_thicnless(data, big_circle, min_circle, bmih, color);

    free(min_circle.points);
    free(big_circle.points);

    return 0;

}

int rectangle_ornament(Rgb*** data, BitmapInfoHeader bmih, object_t figure, Rgb color) {
    int thickness = (figure.thinckness  % 2 == 0) ? figure.thinckness + 1 : figure.thinckness;
    int diff = thickness/2;

    for (int i = 0; i < figure.thinckness; i++) {

        const object_t tmp_figure = {
            .fill = figure.fill,
            .thinckness = figure.thinckness,
            .x_left_up = diff,
            .x_right_down = bmih.width - diff,
            .y_left_up = bmih.height - diff,
            .y_right_down = diff
        };

        draw_rectangle(data, bmih, tmp_figure, (Rgb){0,0,0}, color); // Второй цвет никогда не будет использован
        diff += 2*thickness;
    }
    return 1;
}

int circle_ornament(Rgb*** data, BitmapInfoHeader bmih, Rgb color) {
    int x_c = (bmih.width + 1) / 2;
    int y_c = (bmih.height + 1) / 2;
    int min_radius = x_c < y_c ? x_c : y_c;
    int max_radius =  x_c + y_c;

    object_t tmp_figure = {
        .fill = false,
        .thinckness = 2*(max_radius - min_radius),
        .x_center = x_c,
        .y_center = y_c,
        .radius = max_radius,
    };

    draw_circle(data, bmih, color, (Rgb){0,0,0}, tmp_figure);

}

int semi_circle_ornament(Rgb*** data, BitmapInfoHeader bmih, int count,
                         int thickness, Rgb color)
{
    // Calculate radii for the ornament elements
    const int width_step = (bmih.width + count * 2 - 1) / (count * 2);
    const int height_step = (bmih.height + count * 2 - 1) / (count * 2);

    int current_x = width_step;
    int current_y = bmih.height - height_step - 1;

    const Rgb bg_color = {0, 0, 0};
    const bool no_fill = false;

    for (int i = 0; i < count; i++) {

        object_t horizontal_circle = {
            .thinckness = thickness,
            .fill = no_fill,
            .radius = width_step,
            .x_center = current_x,
            .y_center = 0  // Top
        };
        draw_circle(data, bmih, color, bg_color, horizontal_circle);

        horizontal_circle.y_center = bmih.height;  // Bottom
        draw_circle(data, bmih, color, bg_color, horizontal_circle);

        object_t vertical_circle = {
            .thinckness = thickness,
            .fill = no_fill,
            .radius = height_step,
            .x_center = 0,
            .y_center = current_y
        };
        draw_circle(data, bmih, color, bg_color, vertical_circle);

        vertical_circle.x_center = bmih.width;
        draw_circle(data, bmih, color, bg_color, vertical_circle);

        current_x += 2 * width_step;
        current_y -= 2 * height_step;
    }

    return 0;
}


int canvas_logic(object_t figure) {
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(figure.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color_fill = (Rgb){figure.color_fill_b, figure.color_fill_g, figure.color_fill_g};
    Rgb color =  (Rgb){figure.color_b, figure.color_g, figure.color_r};

    switch (figure.mod) {
        case rect:
            draw_rectangle(&data, bmih, figure, color_fill, color);
            break;
        case ornament:
            switch (figure.pattern) {
                case circle:
                    circle_ornament(&data, bmih, color);
                    break;
                case rectangle:
                    rectangle_ornament(&data, bmih, figure, color);
                    break;
                case semicircle:
                    semi_circle_ornament(&data, bmih, figure.count, figure.thinckness, color);
                    break;
            }

            break;
        case rotate:
            data = rotate_area(&data, bmih, figure);
            if (data == NULL)
                return 1;
            break;
        case circ:
            draw_circle(&data, bmih, color, color_fill, figure);
            break;
        case info:
            print_file_header(bmfh);
            print_info_header(bmih);
            break;
    }


    // Записываем измененное изображение
    if (!write_bmp(figure.finish_filename, &bmfh, &bmih, data)) {
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < (bmih.height); i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}

/*
___________________________CLI_TOOOLS_________________________________________________________
 */

/**
 * @brief Функция конструктор заполняет структуру базовыми значениями, предполагается, что такие значения не могут
 * получится в случае парсера, поэтому можно будет проверить, ввел ли пользователь тому или иному полю значение.
 *
 * Данная функция получая на вход структуру заполняет её поля некоторыми - невозможными значениями, для того чтобы
 * при проверке структуры на корректность переданных аргументов можно было понять, было ли заполнено пользователем
 * или иное поле. Используется при создании структуры для заполнения её базовыми значениями. Не всегда он нужен, тк
 * иногда может заранее подразумеватся, что структура заполненна корректно.
 *
 * @param obj Объект, который нужно инициализировать.
 */
void constuctor(object_t* obj) {
    obj->mod = None;
    obj->start_filename = NULL;
    obj->finish_filename = "out.bmp";
    obj->x_left_up = base_coords_val;
    obj->y_left_up = base_coords_val;
    obj->x_right_down = base_coords_val;
    obj->y_right_down = base_coords_val;
    obj->pattern = none;
    obj->count = base_count_val;
    obj->angle = base_angle_val;
    obj->x_center = base_coords_val;
    obj->y_center = base_coords_val;
    obj->radius = base_radius_val;
    obj->thinckness = base_thinckness_val;
    obj->fill = false;
    obj->color_fill_b = base_color_val;
    obj->color_fill_r = base_color_val;
    obj->color_fill_g = base_color_val;
    obj->color_r = base_color_val;
    obj->color_g = base_color_val;
    obj->color_b = base_color_val;
}

/**
 * @brief Данная функция является справкой для команд, которые поддерживает приложение.
 */
void help_print(void) {
    printf("╭━━╮╭━╮╭━┳━━━╮╭━━━┳━━━┳━━┳━━━━┳━━━┳━━━╮\n"
            "┃╭╮┃┃┃╰╯┃┃╭━╮┃┃╭━━┻╮╭╮┣┫┣┫╭╮╭╮┃╭━╮┃╭━╮┃\n"
            "┃╰╯╰┫╭╮╭╮┃╰━╯┃┃╰━━╮┃┃┃┃┃┃╰╯┃┃╰┫┃╱┃┃╰━╯┃\n"
            "┃╭━╮┃┃┃┃┃┃╭━━╯┃╭━━╯┃┃┃┃┃┃╱╱┃┃╱┃┃╱┃┃╭╮╭╯\n"
            "┃╰━╯┃┃┃┃┃┃┃╱╱╱┃╰━━┳╯╰╯┣┫┣╮╱┃┃╱┃╰━╯┃┃┃╰╮\n"
            "╰━━━┻╯╰╯╰┻╯╱╱╱╰━━━┻━━━┻━━╯╱╰╯╱╰━━━┻╯╰━╯\n");
    printf("\n\n Справка о программе!\n");

    printf("Данная программа обладает флагами:\n"
           "    -h, --help                                                  Печатает информационную справку.\n"
           "    --info <file.bmp>                                           Печатает информацию о bmp файле.\n\n"
           "    --rect  [options]\n"
           "        -l --left_up <x.y>                                      Устанавливает координаты левой точки.\n"
           "        -r --right_down <x.y>                                   Устанавливает координаты правой нижней точки.\n"
           "        -t --thickness <num>                                    Устанавливает толщину линий.\n"
           "        -с --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
           "        -f --fill                                               Устанавливает нужна ли заливка.\n"
           "        -F --fill_color <rrr.ggg.bbb>                           Устанавливает цвета заливки.\n\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --ornament [options] \n"
           "        -p --pattern [rectangle, circle, cemicircles]            Устанавливает режим орнамента.\n"
           "        -t --thickness <num>                                    Устанавливает тольщину линии.\n"
           "        -c --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
           "        -C --count <num>                                        Устанавливает количество.\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --rotate [options] \n"
           "        -l --left_up <x.y>                                      Устанавливает координаты левой точки.\n"
           "        -r --right_down <x.y>                                   Устанавливает координаты правой нижней точки.\n"
           "        -a --angle <num>                                        Устанавливает угол, может быть 90, 180, 270\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --circle [options]\n"
           "        -n --center <x.y>                                       Устанавливает центр окружности.\n"
           "        -R --radius <num>                                       Устанавливает радиус окружности.\n"
           "        -t --thickness <num>                                    Устанавливает толщину линий.\n"
           "        -c --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
           "        -f --fill                                               Устанавливает нужна ли заливка.\n"
           "        -F --fill_color <rrr.ggg.bbb>                           Устанавливает цвета заливки.\n\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n");
}

/**
 * @brief Данная функция проверяет корректность аргументов, проверяет что строку полностью состоит из чисел
 *
 * Функция, которая проверят, что переданная строка является числом.
 * Используется в других функция при проверке на корректность аргументов.
 *
 * @param str строка, которую нужно проверить
 * @return Если число, то возвращает единицу.
 */
bool is_number(const char *str) {
    if (!str || *str == '\0') return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-') return 0;
        str++;
    }
    return 1;
}

/**
 * @brief Данная функция проверяет корректность аргументов парсера, которые заданы через точку (x.y).
 *
 * Функция проверят, что количество точек в данной строке соответствует нужному количеству.
 * В контексте данной программы она проверят, что строка соответствует формату <x.y>. То есть используется при
 * проверке корректности координат, и цветов. В основном используется как вспомогательная функция.
 *
 * @param str Строка, содержимое которое необходимо проверить на корректность.
 * @param count_dots Количество точек, которое должно быть в строке.
 * @return
 */
bool is_correct_dots(const char *str, int count_dots) {
    char* dot = strchr(str, '.');

    for (int i = 0; i < count_dots; i++) {
        if (!dot) return 0;   // Если точек меньше, то завершаем программу.
        dot = strchr(dot + 1, '.');
    }

    // Проверка на корректность символов.
    if (!str || *str == '\0')
        return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-' && *str != '.') return 0;
        str++;
    }

    return !(dot == NULL);
}

/**
 * @brief Вспомогательная функция, помогает определить корректность цвета, диапазон, и количество считанного.
 *
 *  Данная функция проверяет корректность переданных значений цветов.
 *  Используется как вспомогательная функция.
 *
 * @param count Количество прочитанных аргументов.
 * @param red Число красного цвета.
 * @param green Число зелёного цвета.
 * @param blue Число синего цвета.
 * @return Булевое значение.
 */
bool check_colors( int red, int green, int blue) {
    if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
        fprintf(stderr, "Error: цвета переданы в неккоректном формате! Каждый должен быть в диапазоне [0, 255]");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция проверяет, имеет ли файл расширение .bmp.
 *
 * Данная функция проверят, является ли корректным расширение файла, в данном случае bmp.
 * Используется как вспомогательная функция при проверке переданных аргументов.
 *
 * @param filename Имя файла.
 * @return Да или нет
 */
bool check_dt_bmp(const char* filename) {
    size_t len = strlen(filename);
    return !(len >= 4 && strcmp(filename + len - 4, ".bmp") == 0);
}

bool set_mode(const char* mode, object_t* figure){
    if(figure->mod != None)
        return 1;
    if( strcmp(mode, "rect") == 0)
        figure->mod = rect;
                  
    if( strcmp(mode, "ornament") == 0)     
        figure->mod = ornament; 
              

    if( strcmp(mode, "circle") == 0)       
        figure->mod = circ;
              
    if( strcmp(mode, "rotate") == 0)        
        figure->mod = rotate; 


    if( strcmp(mode, "info") == 0)         
        figure->mod = info; 

    return 0;

}

bool parce_pattern(char* optarg, object_t* figure) {
    if (!strcmp(optarg, "rectangle")) figure->pattern = rectangle;
    if (!strcmp(optarg, "semicircles")) figure->pattern = semicircle;
    if (!strcmp(optarg, "circle")) figure->pattern = circle;

    if (figure->pattern == none) {
        fprintf(stderr, "Error: Вы выбрали несушествующий режим орнамента!\n");
        return 1;
    }

    return 0;
}

bool parce_left_up(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (count_read != 2) {
        fprintf(stderr, "Error: координаты переданы в неправильном формате!\n");
        return 1;
    }

    figure->x_left_up = x; figure->y_left_up = y;

    return 0;
}

bool parce_right_down(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (count_read != 2) {
        fprintf(stderr, "Error: координаты переданы в неправильном формате!\n");
        return 1;
    }

    figure->x_right_down = x; figure->y_right_down = y;

    return 0;
}

bool parce_angle(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели не число для --angle\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->angle);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --angle\n");
        return 1;
    }

    if (figure->angle != 90 && figure->angle != 180 && figure->angle != 270) {
        fprintf(stderr, "Error: Введёно неправильно значение для --angle должны быть 90 или 180 или 270.\n");
        return 1;
    }

    return 0;
}

bool parce_count(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: аргумент для --count должен быть числом!\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->count);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --count\n");
        return 1;
    }

    if (figure->count <=0 ) {
        fprintf(stderr, "Error: Введены отрицательные аргументы для --count\n");
        return 1;
    }

    return 0;
}

bool parce_thickness(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --thickness\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->thinckness);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --thinckness\n");
        return 1;
    }

    if (figure->thinckness <= 0) {
        fprintf(stderr, "Error: Неккоректный аргумент для --thickness, должно быть больше нуля!\n");
        return 1;
    }

    return 0;
}

bool parce_color(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 2)) {
        fprintf(stderr, "Error: Цвет передан в некоректном формате!\n");
        return 1;
    }

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (count_read != 3) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --color\n");
        return 1;
    }

    if (check_colors(red, green, blue))
        return 1;

    figure->color_r = red; figure->color_g = green; figure->color_b = blue;
    return 0;
}

bool parce_fill_color(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 2)) {
        fprintf(stderr, "Error: Цвет передан в некоректном формате!\n");
        return 1;
    }

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (count_read != 3) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --fill_color\n");
        return 1;
    }

    if (check_colors(red, green, blue))
        return 1;

    figure->color_fill_r = red; figure->color_fill_g = green; figure->color_fill_b = blue;
    return 0;
}

bool parce_center(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (count_read != 2) {
        fprintf(stderr, "Error: координаты переданы в неправильном формате!\n");
        return 1;
    }

    figure->x_center = x; figure->y_center = y;

    return 0;
}

bool parce_radius(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --radius\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->radius);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --radius\n");
        return 1;
    }

    if (figure->radius <= 0) {
        fprintf(stderr, "Error: Неккоректный аргумент для --radius, должно быть больше нуля!\n");
        return 1;
    }

    return 0;

}

/**
 * @brief Данная функция является логической функции парсера, которая проверяет все команды
 * и передаёт их в зависимые функции.
 *
 * Данная функция является главной логической функцией, которая занимается парсингом всех переданных аргументов в
 * программу. Она представляет собой инициализированный список флагов, а затем ветвистую стркутуру, которая
 * в зависимости от считанного режима запускает выполнение того или иного флага.
 * Используется в программе для парсинга аргументов.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool base_parser(object_t* figure, int argc, char** argv) {

    if (argc == 1){ 
        help_print();
        return 1;
    }   
       
    static struct option long_options[] = {
        {"rect", no_argument, 0, 0},  
        {"ornament", no_argument, 0, 0}, 
        {"rotate", no_argument, 0, 0},  
        {"circle", no_argument, 0, 0},  
        {"info", no_argument, 0, 0},
        {"help", no_argument, 0, 'h'},  
        {"pattern", required_argument, 0, 'p'},
        {"left_up", required_argument, 0, 'l'}, 
        {"right_down", required_argument, 0, 'r'}, 
        {"thickness", required_argument, 0, 't'},
        {"output", required_argument, 0, 'o'}, 
        {"input", required_argument, 0, 'i'}, 
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {"angle", required_argument, 0, 'a'}, 
        {"color", required_argument, 0, 'c'}, 
        {"count", required_argument, 0, 'C'},
        {"center", required_argument, 0, 'n'},
        {"radius", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "hp:l:r:t:o:i:fF:a:c:C:n:R:", long_options, &option_index)) != -1) {
        if (opt == -1) {
            break; // Конец аргументов
        }

        switch (opt) {
            case 0:
                if(set_mode(long_options[option_index].name, figure)){
                    fprintf(stderr, "Error: Программа за один запуск может выполнить только одну функцию!\n");
                    return 1;
                }
                break;
            
            case 'h':
                help_print();
                return 1;
            
            case 'p':
                if (parce_pattern(optarg, figure)) return 1;
                break;

            case 'l':
                if (parce_left_up(optarg, figure))
                    return 1;
                break;
            
            case 'r':
                if (parce_right_down(optarg, figure))
                    return 1;
                break;

            case 'a':
                if (parce_angle(optarg, figure))
                    return 1;
                break;

            case 'i':
                figure->start_filename = optarg;
                break;

            case 'o':
                figure->finish_filename = optarg;
                break;

            case 'C': {
                if (parce_count(optarg, figure))
                    return 1;
                break;
            }
            case 't': {
                if (parce_thickness(optarg, figure))
                    return 1;
                break;
            }
            case 'c': {
                if (parce_color(optarg, figure))
                    return 1;
                break;
            }

            case 'f':
                figure->fill = true;
                break;
            case 'F':
                if (parce_fill_color(optarg, figure))
                    return 1;
                break;

            case 'n':
                if (parce_center(optarg, figure))
                    return 1;
                break;

            case 'R':
                if (parce_radius(optarg, figure))
                    return 1;
                break;

            case '?':
                fprintf(stderr, "Error: Вы ввели неккоректную опцию!\n");
                return 1;
            
            default:
                fprintf(stderr, "Error: Ошибка парсинга аргументов!\n");
                return 1;
        }
    }

    if (optind + 1 == argc && figure->start_filename == NULL) {
        figure->start_filename = argv[optind];
        return 0;
    }

    
    if (optind  < argc){
        fprintf(stderr, "Error: Введены лишние аргументы!\n");
        while (optind < argc) 
            fprintf(stderr, "Error: лишний аргумент - %s\n", argv[optind++]);
        return 1;
    }

    return 0;
}


/**
 * @brief Проверят на корректность заполненость цветов для определённого режима.
 *
 * Данная функция проверяет на корректность заполненность полей цвета в переданной структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на объект
 * @return Правда или ложь.
 */
bool checker_color(object_t* figure) {
    if (figure->color_r == -1 || figure->color_g == -1 || figure->color_b == -1) {
        fprintf(stderr, "Error: Вы не ввели цвет для фигуры!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненность поля thinckness.
 *
 * Данная функция проверяет корректность заполненности поля thinckness в переданной ей структуре. Проверяет
 * инициализированно ли поле, а также на то, больше нуля ли значение поля.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_thinckness(object_t* figure) {
    if (figure->thinckness == -1) {
        fprintf(stderr, "Error: Вы не ввели толщину линии!\n");
        return 1;
    }

    if (figure->thinckness <= 0) {
        fprintf(stderr, "Error: Передана неккоректная толщина линии, должна быть больше нуля!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненность полей координат левой точки.
 *
 * Данная функция проверяет инициализированные ли координаты в переданной ей структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_left_up(object_t* figure) {
    if (figure->x_left_up == INT_MIN || figure->y_left_up == INT_MIN) {
        fprintf(stderr, "Error: Вы не ввели координаты для левой верхней точки!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненность полей координат правой точки.
 *
 * Данная функция проверяет инициализированные ли координаты в переданной ей структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_right_down(object_t* figure) {
    if (figure->x_right_down == INT_MIN || figure->y_right_down == INT_MIN) {
        fprintf(stderr, "Error: Вы не ввели координаты для правой нижней точки!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненность полей заливки и цвета заливки.
 *
 * Данная функция проверяет инициализированные ли цвета в переданной ей структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_fill(object_t* figure) {
    if (figure->fill && (figure->color_fill_b == -1 || figure->color_fill_r == -1 || figure->color_fill_g == -1)) {
        fprintf(stderr, "Error: Вы выбрали режим заливки, но не выбрали цвет!\n");
        return 1;
    }

    if (!figure->fill && (figure->color_fill_b != -1 || figure->color_fill_r != -1 || figure->color_fill_g != -1)) {
        fprintf(stderr, "Error: Вы не выбрали режим заливки, но выбрали цвет!\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция проверяет корректность заполненности структуры для режима работы --rect.
 *
 * Данная функция проверяет инициализированны ли поля необходимые для режима  --rect в переданной ей структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на структуру.
 * @return Ноль или единица
 */
bool check_rectangle(object_t* figure) {
    if (checker_left_up(figure)) return 1;
    if (checker_right_down(figure)) return 1;
    if (checker_thinckness(figure)) return 1;
    if (checker_color(figure)) return 1;
    if (checker_fill(figure)) return 1;

    if (figure->y_center != base_coords_val ||  figure->x_center != base_coords_val) return 2;
    if (figure->radius != base_radius_val ||  figure->angle != base_angle_val) return 2;
    if (figure->pattern != none ||  figure->count != base_count_val) return 2;

    return 0;
}

/**
 * @brief Данная функция проверяет корректность заполнености структуры для режима работы --circle.
 *
 * Данная функция проверяет инициализированны ли поля необходимые для режима  --circle в переданной ей структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на структуру.
 * @return Ноль или единица
 */
bool check_circle(object_t* figure) {
    // Проверяем специфические параметры
    if (figure->x_center == INT_MIN || figure->y_center == INT_MIN) {
        fprintf(stderr, "Error: Вы не выбрали центр окружности!\n");
        return 1;
    }

    if (figure->radius == -1 ) {
        fprintf(stderr, "Error: Вы не выбрали радиус окружности!\n");
        return 1;
    }

    if (checker_thinckness(figure)) return 1;
    if (checker_color(figure)) return 1;
    if (checker_fill(figure)) return 1;

    if (figure->x_right_down != base_coords_val ||  figure->x_left_up != base_coords_val) return 2;
    if (figure->y_right_down != base_coords_val ||  figure->y_left_up != base_coords_val) return 2;
    if (figure->pattern != none ||  figure->count != base_count_val || figure->angle != base_angle_val) return 2;
    return 0;
}

/**
 * @brief Проверят корректность полей для --ornament.
 *
 * Данная функция проверяет инициализированны ли поля необходимые для режима  --ornament в переданной ей структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на объект.
 * @return Ноль в случае удачи в ином случае единица.
 */
int check_ornament(object_t* figure) {

    if (figure->color_r == base_color_val || figure->color_g == base_color_val || figure->color_b == base_color_val) {
        fprintf(stderr, "Error: Вы не ввели цвет для фигуры!\n");
        return 1;
    }

    if (figure->y_center != base_coords_val ||  figure->x_center != base_coords_val) return 2;
    if (figure->radius != base_radius_val ||  figure->angle != base_angle_val) return 2;
    if (figure->fill != false ||  figure->color_fill_b != base_color_val ||  figure->color_fill_g != base_color_val ||  figure->color_fill_r != base_color_val) return 2;
    if (figure->x_right_down != base_coords_val ||  figure->x_left_up != base_coords_val) return 2;
    if (figure->y_right_down != base_coords_val ||  figure->y_left_up != base_coords_val) return 2;


    switch (figure->pattern) {
        case rectangle:
            if (checker_color(figure)) return 1;
            if (checker_thinckness(figure)) return 1;

            if (figure->count == base_count_val) {
                fprintf(stderr, "Error: Вы не ввели количество!\n");
                return 1;
            }
            break;
        case circle:
            if (figure->thinckness != base_thinckness_val || figure->count != base_count_val) return 2;
            break;
        case semicircle:
            if (checker_color(figure)) return 1;
            if (checker_thinckness(figure)) return 1;
            if (figure->count == base_count_val) {
                fprintf(stderr, "Error: Вы не ввели количество фигур!\n");
                return 1;
            }
            break;
        case none:
            fprintf(stderr, "Error: не был введён орнамента режим!\n");
            return 1;
        default:
            fprintf(stderr, "Error: Невозможная ошибка в проверка орнамента.\n");
            return 1
        ;
    }

    return 0;
}

/**
 * @brief Проверят корректность полей для --rotate.
 *
 * Данная функция проверяет инициализированны ли поля необходимые для режима  --rotate в переданной ей структуре.
 * Используется как вспомогательная функция.
 *
 * @param figure Указатель на объект.
 * @return Ноль в случае удачи в ином случае единица.
 */
int check_rotate(object_t* figure) {
    if (checker_left_up(figure)) return 1;

    if (checker_right_down(figure)) return 1;

    if (figure->angle == -1 ) {
        fprintf(stderr, "Error: Вы не выбрали угол поворота!\n");
        return 1;
    }

    if (figure->y_center != base_coords_val ||  figure->x_center != base_coords_val) return 2;
    if (figure->radius != base_radius_val ||  figure->count != base_count_val) return 2;
    if (figure->pattern != none ||  figure->count != base_count_val) return 2;
    if (figure->fill != false ||  figure->color_fill_b != base_color_val ||  figure->color_fill_g != base_color_val ||  figure->color_fill_r != base_color_val) return 2;
    if (figure->thinckness != base_thinckness_val ||  figure->color_b != base_color_val ||  figure->color_g != base_color_val ||  figure->color_r != base_color_val) return 2;

    return 0;
}

int check_info(object_t* figure) {
    if (figure->y_center != base_coords_val ||  figure->x_center != base_coords_val) return 2;
    if (figure->fill != false ||  figure->color_fill_b != base_color_val ||  figure->color_fill_g != base_color_val ||  figure->color_fill_r != base_color_val) return 2;
    if (figure->radius != base_radius_val ||  figure->count != base_count_val ||  figure->angle != base_angle_val) return 2;
    if (figure->pattern != none ||  figure->count != base_count_val) return 2;
    if (figure->thinckness != base_thinckness_val ||  figure->color_b != base_color_val ||  figure->color_g != base_color_val ||  figure->color_r != base_color_val) return 2;
    if (figure->x_right_down != base_coords_val ||  figure->x_left_up != base_coords_val) return 2;
    if (figure->y_right_down != base_coords_val ||  figure->y_left_up != base_coords_val) return 2;

    return 0;
}

/**
 * @brief Данная функция является проверкой корректности переданнх значений, соответствие режимам и т.п.
 * Простые проверки производит сама, но более ветвистые перераспределяет по зависимым вспомогательным функциям.
 *
 * Данная функция является главной функцией для проверки корректности заполенности полей переданного экземпляра
 * структуры. Проверят пустое ли поле названия входного файла. На совпадение названия начального и конечного названия.
 * проверят на корректность расширения.
 * Используется как функция проверки заполненности структуры.
 *
 * @param figure Указатель на проверяемый объект
 * @return Если все проверки прошли, то ноль, в ином случае код ошибки.x
 */
int base_checker(object_t* figure) {
    if (figure->start_filename == NULL) {
        fprintf(stderr, "Error: Не было введено имя файла!\n");
        return 1;
    }

    if (strcmp(figure->start_filename, figure->finish_filename) == 0) {
        fprintf(stderr, "Error: Начальные и стартовые имена совпадают!\n");
        return 1;
    }

    if (check_dt_bmp(figure->start_filename)) {
        fprintf(stderr, "Error: Изначальный файл не имеет расширение .bmp!\n");
        return 1;
    }

    if (check_dt_bmp(figure->finish_filename)) {
        fprintf(stderr, "Error: Конечный файл не имеет расширение .bmp!\n");
        return 1;
    }

    int res_check = 0;
    switch (figure->mod) {
        case rect:
            res_check = check_rectangle(figure);
            break;
        case ornament:
            res_check = check_ornament(figure);
            break;
        case rotate:
            res_check = check_rotate(figure);
            break;
        case circ:
            res_check = check_circle(figure);
            break;
        case info:   // Избыточно, так как уже проверяется корректность имени файла.
            res_check = check_info(figure);
            break;
        case help:
            printf("sosat\n");
            break;
        default:
            fprintf(stderr, "Error: Не выбран режим работы!\n");
            return 1;
    }

    return res_check;
}

int main(int argc, char* argv[]){
    printf("Course work for option 5.1, created by Ivan Aleinikov\n");

    object_t* figure = malloc(sizeof(object_t));
    constuctor(figure);

    if (base_parser(figure, argc, argv)) return 41;

    switch (base_checker(figure)) {
        case 1:
            return 42;
        case 2:
            fprintf(stderr, "Error: были переданны неккоректные параметры для выборанной вами функци!\n");
            return 43;
    }

    canvas_logic(*figure);

    free(figure);

    return 0;
}


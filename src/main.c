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

/**
 * @brief Данное перечисление является хранилищем режимов для функции comp_cnt_args.
 *
 * В данном перечислении находятся константы, которые указывают, сколько аргументов должна была считать
 * программа. Используется в случае, если нужно проверить, сколько аргументов считала программа.
 */
enum count_arguments {one_read = 1, two_read = 2, three_read = 3};



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
    if((x >= 0 && x <= weight) && (y >= 0 && y <= height))
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

int draw_rectangle(object_t info_m){
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(info_m.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color = {info_m.color_b, info_m.color_g, info_m.color_r};
    Rgb color_thinckless = {info_m.color_fill_b, info_m.color_fill_g, info_m.color_fill_r};

    for(int y = info_m.y_right_down - info_m.thinckness; y <= info_m.y_left_up + info_m.thinckness; y++){
        for(int x = info_m.x_left_up - info_m.thinckness; x <= info_m.x_right_down + info_m.thinckness; x++){

            if(check_draw_thincless(y, x, info_m.thinckness, info_m.x_left_up, info_m.x_right_down, info_m.y_right_down, info_m.y_left_up) && check_coord(y, x, bmih.height, bmih.width))
                    data[y][x] = color_thinckless;

            if(info_m.fill){
                if(x <= info_m.x_right_down && x >= info_m.x_left_up && y >= info_m.y_right_down && y <= info_m.y_left_up && check_coord(y, x, bmih.height, bmih.width))
                    data[y][x] = color;
            }

        }
    }

    // Записываем измененное изображение
    if (!write_bmp(info_m.finish_filename, &bmfh, &bmih, data)) {
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < (bmih.height); i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}



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
int rotate_area(object_t figure){
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(figure.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb** new_data = copy_array(data, bmih.height, bmih.width);
    int x_center = (figure.x_left_up + figure.x_right_down)/2;
    int y_center = (figure.y_left_up + figure.y_right_down)/2;


    for (int y = figure.y_right_down; y <= figure.y_left_up; y++) {
        for (int x = figure.x_left_up; x <= figure.x_right_down; x++) {
            // Смещаем пиксель относительно центра
            int x_shifted = x - x_center;
            int y_shifted = y - y_center;

            // Поворот
            int x_new = (int)((x_shifted * custom_cos(figure.angle) + y_shifted * custom_sin(figure.angle)) + x_center);
            int y_new = (int)((-x_shifted *custom_sin(figure.angle)) + y_shifted * custom_cos(figure.angle) + y_center);

            if(check_coord(x_new, y_new, bmih.height, bmih.width)) {
                new_data[y_new][x_new] = data[y][x];
            }


        }
    }


    // Записываем измененное изображение
    if (!write_bmp(figure.finish_filename, &bmfh, &bmih, new_data)){
        return 1;
    }

    // Освобождаем память
    for (int i = 0; i < (bmih.height); i++) {
        free(new_data[i]);
        free(data[i]);
    }
    free(data);
    free(new_data);

    return 0;
}

int draw_circle(object_t info_m){
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    Rgb **data = NULL;

    if(!read_bmp(info_m.start_filename, &bmfh, &bmih, &data)){
        fprintf(stderr, "Error: Не удалось считать данные из файла.\n");
        return 0;
    }

    Rgb color = {info_m.color_b, info_m.color_g, info_m.color_r};
    Rgb color_thinckless = {info_m.color_fill_b, info_m.color_fill_g, info_m.color_fill_r};

    for(int y = info_m.y_center - info_m.radius - info_m.thinckness + 1; y < info_m.y_center + info_m.radius + info_m.thinckness; y++){
        for(int x = info_m.x_center - info_m.radius - info_m.thinckness + 1; x < info_m.x_center + info_m.radius + info_m.thinckness; x++){

            int radius_pre =(x - info_m.x_center)*(x - info_m.x_center) + (y - info_m.y_center)*(y - info_m.y_center);
            int min_rad =  info_m.radius*info_m.radius;
            int max_ras = (info_m.radius + info_m.thinckness)*(info_m.radius + info_m.thinckness);


            if(radius_pre <= max_ras && radius_pre >= min_rad && check_coord(y, x, bmih.height, bmih.width))
                data[y][x] = color_thinckless;
                

            if(info_m.fill){
                if(radius_pre < min_rad && check_coord(y, x, bmih.height, bmih.width))
                    data[y][x] = color;
            }   

        }
    }

    // Записываем измененное изображение
    if (!write_bmp(info_m.finish_filename, &bmfh, &bmih, data)) {
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
    obj->x_left_up = INT_MIN;
    obj->y_left_up = INT_MIN;
    obj->x_right_down = INT_MIN;
    obj->y_right_down = INT_MIN;
    obj->pattern = none;
    obj->count = -1;
    obj->angle = -1;
    obj->x_center = INT_MIN;
    obj->y_center = INT_MIN;
    obj->radius = -1;
    obj->thinckness = -1;
    obj->fill = false;
    obj->color_fill_b = -1;
    obj->color_fill_r = -1;
    obj->color_fill_g = -1;
    obj->color_r = -1;
    obj->color_g = -1;
    obj->color_b = -1;
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
           "    --info <file.bmp>                                       Печатает информацию о bmp файле.\n\n"
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
           "        -p --pattern [rectangle, circle, cemicircle]            Устанавливает режим орнамента.\n"
           "        -t --thickness <num>                                    Устанавливает тольщину линии.\n"
           "        -C --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
           "        -c --count <num>                                        Устанавливает количество.\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --rotate [options] \n"
           "        -l --left_up <x.y>                                      Устанавливает координаты левой точки.\n"
           "        -r --right_down <x.y>                                   Устанавливает координаты правой нижней точки.\n"
           "        -a --angle <num>                                        Устанавливает угол, может быть 90, 180, 270\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --circle [options]\n"
           "        -c --center <x.y>                                       Устанавливает центр окружности.\n"
           "        -r --radius <num>                                       Устанавливает радиус окружности.\n"
           "        -t --thickness <num>                                    Устанавливает толщину линий.\n"
           "        -С --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
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
 * @brief Данная функция проверят, передал ли пользователь лишний аргумент, после флага, не требующего аргументы.
 *
 *  Функция проверят, что аргументов больше двух (в противном случае доп аргумента попросту и нету)
 *  затем она проверят, что следующий аргумент не флаг, в случае неудачи возвращает ложь.
 *  Используется как вспомогательная функция.
 *
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Булевое значение.
 */
bool no_arguments(int argc, char** argv, char* flag_name) {
    if (argc > 2 && argv[2][0] != '-') {
        fprintf(stderr, "Error:  %s не принимает аргументов!\n", flag_name);
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверят есть ли лишний аргумент после функции.
 *
 * Данная функция проверяет, для какого-то элемента не передан ли лишний элемент.
 * Проверяет, на то, является ли аргумент числом. А также проверяет, что следующий аргумент - это не флаг.
 *
 * @param argc Количество аргументов.
 * @param argv Массив указателей на аргументы.
 * @param name Имя функции, где произошла проверка.
 * @return Единицу, если слишком много аргументов, ноль, если все в норме.
 */
bool is_correct_count_args(int argc, char** argv, char* name, char* start_filename ) {
    if (!optarg) {
        fprintf(stderr, "Error: Не был передан аргумент для %s!\n", name);
        return 1;
    }

    if (optarg[0] == '-' && !isdigit(optarg[1])) {
        fprintf(stderr, "Error: вы не ввели аргументы для функции %s!\n", name);
        return 1;
    }


    printf("%s %d argc %d f1 %d\n", name, optind, argc, isalpha(argv[optind][0]));
    if ( optind < argc && argv[optind][0] != '-' && (optind  + 1 != argc && start_filename == NULL)){
        fprintf(stderr, "Error: вы ввели слишком много аргументов для %s\n", name);
        return 1;
    }

    return 0;
}

/**
 * @brief Функция сравнения считанных аргументов, и ожидаемого количества.
 *
 * Данная функция проверяет, совпадает ли количество аргументов,
 * переданных в программу, с ожидаемым количеством. Если количество
 * считанных аргументов не соответствует ожидаемому, функция
 * выводит сообщение об ошибке в стандартный поток ошибок
 * и возвращает значение 1 (истина). В противном случае
 * возвращает 0 (ложь).
 *
 * @param read_count Общее количество аргументов, которые были прочитаны.
 * @param correct_count Ожидаемое количество аргументов.
 * @param name_mode Имя режима работы программы, используемое для
 * вывода в сообщение об ошибке.
 *
 * @return true (1), если количество аргументов не совпадает,
 * иначе false (0).
 */
bool comp_cnt_args(int read_count, int correct_count, char* name_mode) {
    if (read_count != correct_count) {
        fprintf(stderr, "Error: Некорректно переданы аргументы для  %s\n", name_mode);
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция осуществляет проверку на корректность аргументов - цвета.
 *
 *  Данная функция является некой логической функцией, которая проверяет переданную ею строку но корректность
 *  соответствию формату цвета.
 *  Используется как вспомогательная функция при проверке парсированных аргументов.
 *
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @param name Название флага.
 * @return Если все правильно, то 0, иначе код ошибки.
 */
bool check_color(int argc, char** argv, char* name, char* filename_inp) {
    int count_args = is_correct_count_args(argc, argv, name, filename_inp);
    if (count_args != 0) return count_args;

    if (is_correct_dots(optarg, 2)) {
        fprintf(stderr, "Error: Цвета переданы в некорректном формате! Должно быть - r.g.b\n");
        return 1;
    }

    return 0;
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
bool check_colors(int count_read, int red, int green, int blue) {
    if (comp_cnt_args(count_read, three_read, ""))
        return 1;

    if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
        fprintf(stderr, "Error: цвета переданы в неккоректном формате! Каждый должен быть в диапазоне [0, 255]");
        return 1;
    }

    return 0;
}

/**
 *@brief Данная функция проверят в правильном ли формате переданы координаты точки.
 *
 *  Данная функция проверяет координаты на соответсвие необходимому шаблону функции, подразумевается, что данная
 *  функция будет использоватся для флагов, которые подразумевают получение того или иного аргумента.
 *  Используется при проверки корректности парсинга координат.
 *
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @param name Имя проверяемого флага.
 * @return Ноль, если все в порядке, единицу в противном случае.
 */
bool parce_coords(int argc, char** argv, char* name, char* filename_start) {
    // Проверка на корректность количества аргументов.
    int count_args = is_correct_count_args(argc, argv, name, filename_start);
    if (count_args != 0) return count_args;

    // Вторая проверка, на количество точек, гарантирует, что передано правильно количество аргументов.
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция является реализацией парсера флага --left_up.
 *
 *  Является функцией парсинга флага --left_up. Проверяет аргументы на корректность. Затем, в случае успеха
 *  заносит данные в поля стркутуры.
 *  Исползуется при реализации парсера функции --left_up.
 *
 * @param figure Указатель на объект фигуры.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Возвращает ноль, если все в порядке, в ином случае либо код ошибки, либо единицу.
 */
bool left_up(object_t* figure, int argc, char** argv) {
    int result_coords = parce_coords(argc, argv, "left_up", figure->start_filename);
    if (result_coords != 0)
        return result_coords;

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (comp_cnt_args(count_read, two_read, "--left_up"))
        return 1;

    figure->x_left_up = x; figure->y_left_up = y;

    return 0;
}

/**
 * @brief Данная функция является реализацией парсера флага --right_down.
 *
 *  Является функцией парсинга флага --right_down. Проверяет аргументы на корректность. Затем, в случае успеха
 *  заносит данные в поля стркутуры.
 *  Исползуется при реализации парсера функции --right_down.
 *
 * @param figure Указатель на объект фигуры.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Возвращает ноль, если все в порядке, в ином случае либо код ошибки, либо единицу.
 */
bool right_down(object_t* figure, int argc, char** argv) {
    int result_coords = parce_coords(argc, argv, "right_down", figure->start_filename);
    if (result_coords != 0)
        return result_coords;

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (comp_cnt_args(count_read, two_read, "--right_down"))
        return 1;

    figure->x_right_down = x; figure->y_right_down = y;
    return 0;
}

/**
 * @brief Данная функция явяется реализацией парсера для флага --thickness.
 *
 *  Данная функция является реализацией функцией парсера для флага --thickness. Проверяет переданные ей аргументы.
 *  на корректность.
 *  Используется в функциях для реализации флага --thickness.
 *
 * @param figure Указатель на объект данных.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль, если все в порядке, в ином случает 1.
 */
bool thickness(object_t* figure, int argc, char** argv) {
    int count_args = is_correct_count_args(argc, argv, "--thickness", figure->start_filename);
    if (count_args != 0) return count_args;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --thickness\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->thinckness);

    if (comp_cnt_args(count_read, one_read, "--thickness"))
        return 1;

    if (figure->thinckness <= 0) {
        fprintf(stderr, "Error: Неккоректный аргумент для --thickness, должно быть больше нуля!\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Функция, которая реализует парсинг флага --color.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --color. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --color.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Если все в порядке, то ноль, в ином случае код ошибки.
 */
bool color(object_t* figure, int argc, char** argv) {
    int first_check = check_color(argc, argv, "--color", figure->start_filename);
    if (first_check != 0) return first_check;

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (check_colors(count_read, red, green, blue))
        return 1;

    figure->color_r = red; figure->color_g = green; figure->color_b = blue;
    return 0;
}

/**
 * @brief Функция, которая реализует парсинг флага --fill_color.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --fill_color. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --fill_color.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Если все в порядке, то ноль, в ином случае код ошибки.
 */
bool fill_color(object_t* figure, int argc, char** argv) {
    int first_check = check_color(argc, argv, "--fill_color", figure->start_filename);
    if (first_check != 0) return first_check;

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (check_colors(count_read, red, green, blue))
        return 1;

    figure->color_fill_r = red; figure->color_fill_g = green; figure->color_fill_b = blue;
    return 0;
}

/**
 * @brief Функция, которая реализиет парсинг флага --input.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --input. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --input.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль, если все хорошо, в ином случае единицу.
 */
bool input_name(object_t* figure, int argc, char** argv) {
    int check_args = is_correct_count_args(argc, argv, "--input", figure->start_filename);
    if (check_args != 0) return check_args;

    figure->start_filename = optarg;
    return 0;
}

/**
 * @brief Функция, которая реализует парсинг флага --output.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --output. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --output.
 *
 * @param figure Указатель на фигуру.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Если все нормально ноль, в ином случае код ошибки.
 */
bool output_name(object_t* figure, int argc, char** argv) {
    int check_args = is_correct_count_args(argc, argv, "output_name", figure->start_filename);
    if (check_args != 0) return check_args;

    figure->finish_filename = optarg;
    return 0;
}

/**
 * @brief Функция для получения имени входного файла из конца.
 *
 * Данная функция считывает название файла из конца списка аргументов. Поверят, что вообще есть аргумент. Проверят,
 * что это не флаг. Проверяет, что это не аргумент ранее идущего флага. Проверяет, что  последний флаг --fill.
 * Используется считывании названия файла из конца.
 *
 * @param figure Указатель на фигуру.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 */
void get_filename(object_t* figure, int argc, char** argv) {
    if (figure->start_filename != NULL)
        return;

    bool has_enough_args = (argc - 2 > 0);
    bool previous_is_not_flag = (argv[argc - 2][0] == '-' && isdigit(argv[argc - 2][1])) || argv[argc - 2][0] != '-' ;
    bool only_fill_flag_present = (argv[argc - 2][0] == '-' && argc - 3 == 0);
    bool is_fill_flag = figure->fill &&
                        (strcmp(argv[argc - 2], "--fill") == 0 ||
                         strcmp(argv[argc - 2], "-f") == 0);

    if ((has_enough_args && previous_is_not_flag) ||
        only_fill_flag_present ||
        is_fill_flag) {
        figure->start_filename = argv[argc - 1];
        }
}

/**
 * @brief Функция, которая парсит флаг --angle.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --angle. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --angle.
 *
 * @param figure Указатель на фигуру.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль в случае удачи, код ошибки в ином случае.
 */
bool angle(object_t* figure, int argc, char** argv) {
    if (is_correct_count_args(argc, argv, "--angle", figure->start_filename))
        return 1;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели не число для --angle\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->angle);

    if (comp_cnt_args(count_read, one_read, "--angle"))
        return 1;

    if (figure->angle != 90 && figure->angle != 180 && figure->angle != 270) {
        fprintf(stderr, "Error: Введёно неправильно значение для --angle должны быть 90 или 180 или 270.\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция парсит флаг --circle.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --circle. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --circle.
 *
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль в случае успеха, в ином случае код ошибки.
 */
bool circle_get(object_t* figure, int argc, char** argv) {
    if (parce_coords(argc, argv, "--circle", figure->start_filename))
        return 1;

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (comp_cnt_args(count_read, two_read, "--center"))
        return 1;

    figure->x_center = x; figure->y_center = y;
    return 0;
}

/**
 * @brief Данная функция парсит флаг --radius.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --radius. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --radius.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return В случае успеха ноль, в ином случае код ошибки.
 */
bool radius(object_t* figure, int argc, char** argv) {
    if (parce_coords(argc, argv, "--radius", figure->start_filename))
        return 1;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --radius\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->radius);

    if (comp_cnt_args(count_read, one_read, "--radius"))
        return 1;

    if (figure->radius <= 0) {
        fprintf(stderr, "Ошибка: Радиус должен быть больше нуля! --radius\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция парсит флаг --count.
 *
 * Данная функция является реализации функции-парсера для аргументов флага --count. Проверяет переданные аргументы
 * Используется как функция проверки парсинга флага --count.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return В случае успеха ноль, в ином случае код ошибки.
 */
bool count(object_t* figure, int argc, char** argv) {
    if (is_correct_count_args(argc, argv, "--count", figure->start_filename))
        return 1;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: аргумент для --count должен быть числом!\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->count);

    if (comp_cnt_args(count_read, one_read, "--count"))
        return 1;

    if (figure->count <=0 ) {
        fprintf(stderr, "Error:  аргументы неправильные для --count\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция является парсером --rect.
 *
 * Данная функция является парсером для режима --rect, она считывает переданные после флаги, и посредством ветвления
 * распределяет задачи по вспомогательным функциям.
 * Используется при парсинге режима --rect.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool parce_rectangle(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'c'},
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},

        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "l:r:t:c:fF:i:o:", long_options, &option_index)) != -1) {

        switch (opt) {
            case 'l': {
                if (left_up(figure, argc, argv))
                    return 1;
                break;
            }
            case 'r': {
                if (right_down(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 't': {
                if (thickness(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'c': {
                if (color(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'f':
                if (optind < argc && argv[optind][0] != '-' && !(isalpha(argv[optind][0]) && optind  + 1 == argc)) {
                    fprintf(stderr, "Error: --fill не принимает аргументы!\n");
                    return -1;
                }
                figure->fill = true;
                break;
            case 'F':
                if (fill_color(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'i':
                if (input_name(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'o':
                if (output_name(figure, argc, argv) != 0)
                    return 1;
                break;

            case '?':
                fprintf(stderr, "Error: некорректный аргумент.\n");
                return 1;
        }
    }

    get_filename(figure, argc, argv);
    return 0;
}

/**
 * @brief Данная функция является парсером для --rotate.
 *
 * Данная функция является парсером для режима --rotate, она считывает переданные после флаги, и посредством ветвления
 * распределяет задачи по вспомогательным функциям.
 * Используется при парсинге режима --rotate.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool parce_rotate(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"angle", required_argument, 0, 'a'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "l:r:a:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'l':
                if (left_up(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'r':
                if (right_down(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'a':
                if (angle(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'i':
                if (input_name(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'o':
                if (output_name(figure, argc, argv) != 0)
                    return 1;
                break;

            case '?':
                fprintf(stderr, "Error: некорректный аргумент.\n");
                return 1;
        }
    }

    get_filename(figure, argc, argv);
    return 0;
}

/**
 * @brief Данная функция является парсером для --circle.
 *
 * Данная функция является парсером для режима --circle, она считывает переданные после флаги, и посредством ветвления
 * распределяет задачи по вспомогательным функциям.
 * Используется при парсинге режима --circle.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool parce_circle(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"center", required_argument, 0, 'c'},
        {"radius", required_argument, 0, 'r'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'C'},
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:c:r:C:fF:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c':
                if (circle_get(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'r':
                if (radius(figure, argc, argv) != 0)
                    return 1;
                break;

            case 't':
                if (thickness(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'C':
                if (color(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'f':
                if (optind < argc && argv[optind][0] != '-') {
                    fprintf(stderr, "Error: --fill не принимает аргументы!\n");
                    return 1;
                }
                figure->fill = true;
                break;
            case 'F':
                if (fill_color(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'i':
                if (input_name(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'o': {
                if (output_name(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case '?':
                fprintf(stderr, "Error: некорректный аргумент.\n");
                return 1;
        }
    }

    get_filename(figure, argc, argv);
    return 0;
}

/**
 * @brief Данная функция является парсером для --ornament.
 *
 * Данная функция является парсером для режима --ornament, она считывает переданные после флаги, и посредством ветвления
 * распределяет задачи по вспомогательным функциям.
 * Используется при парсинге режима --ornament.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool parce_ornament(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"pattern", required_argument, 0, 'p'},
        {"count", required_argument, 0, 'c'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'C'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "p:c:t:C:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'p': {
                int count_args = is_correct_count_args(argc, argv, "--pattern", figure->start_filename);
                if (count_args != 0) return count_args;

                if (!strcmp(optarg, "rectangle")) figure->pattern = rectangle;
                if (!strcmp(optarg, "semicircles")) figure->pattern = semicircle;
                if (!strcmp(optarg, "circle")) figure->pattern = circle;

                if (figure->pattern == none) {
                    fprintf(stderr, "Error: Вы не выбрали режим орнамента!\n");
                    return 1;
                }
                break;

            }
            case 'c': {
                if (count(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 't': {
                if (thickness(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'C': {
                if (color(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'i': {
                if (input_name(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'o': {
                if (output_name(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case '?':
                fprintf(stderr, "Error: некорректный аргумент.\n");
                return 1;
        }
    }

    get_filename(figure, argc, argv);
    return 0;
}

// Пока так.
void print_help() {
    printf("Course work for option 5.1, created by Ivan Aleinikov\n");
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

    if (argc == 1)    // Проверка на то, что если не передано аргументов, про программа ничего не делает.
        return 1;

    static struct option long_options[] = {
        {"rect", no_argument, 0, 0},
        {"ornament", no_argument, 0, 0},
        {"rotate", no_argument, 0, 0},
        {"circle", no_argument, 0, 0},
        {"info", required_argument, 0, 0},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;
    opt = getopt_long(argc, argv, "h", long_options, &option_index);

    if (opt == 'h') {
        if (argc != 2) {            // Проверяем, что количество аргументов равно единице.
            fprintf(stderr, "Error: Передано слишком много аргументов.\n");
            return 1;
        }

        help_print();
        return 1;
    }

    if (opt == 0) {
        int res = 0;
        switch (option_index) {
            case rect:

                if (no_arguments(argc, argv, "--rect"))
                    return 1;

                figure->mod = rect;
                res = parce_rectangle(argc, argv, figure);
                break;
            case ornament:
                if (no_arguments(argc, argv, "--ornament"))
                    return 1;

                figure->mod = ornament;
                res = parce_ornament(argc, argv, figure);
                break;
            case rotate:
                if (no_arguments(argc, argv, "--rotate"))
                    return 1;
                figure->mod = rotate;
                res = parce_rotate(argc, argv, figure);
                break;
            case circ:
                if (no_arguments(argc, argv, "--circle"))
                    return 1;
                figure->mod = circ;
                res = parce_circle(argc, argv, figure);
                break;
            case info:
                figure->mod = info;

                if (argc != 3) {        // В данном случае, проверяем, что передано всего два аргумента.
                    fprintf(stderr, "Error: Передано слишком много аргументов.\n");
                    return 1;
                }

                if(!optarg){
                    fprintf(stderr, "Error: не было передано название файла!\n");
                    return 1;
                }

                figure->start_filename = optarg;
                break;

            case help:

                if (argc != 2) {            // Проверяем, что количество аргументов равно единице.
                    fprintf(stderr, "Error: Передано слишком много аргументов.\n");
                    return 1;
                }
                print_help();
                help_print();
                return 1;

            case '?':
                fprintf(stderr, "Error: была переданна неизвестная коменда!\n");
                return 1;
        }
        return res;
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
bool check_ornament(object_t* figure) {

    if (figure->color_r == -1 || figure->color_g == -1 || figure->color_b == -1) {
        fprintf(stderr, "Error: Вы не ввели цвет для фигуры!\n");
        return 1;
    }

    switch (figure->pattern) {
        case rectangle:
            if (checker_color(figure)) return 1;
            if (checker_thinckness(figure)) return 1;

            if (figure->count == -1) {
                fprintf(stderr, "Error: Вы не ввели количество!\n");
                return 1;
            }

            break;
        case circle:
            if (figure->thinckness != -1) {
                fprintf(stderr, "Error: Вы выбрали режим орнамента круг, но добавили лишний аргумент --thinckness!\n");
                return 1;
            }

            if (figure->count != -1) {
                fprintf(stderr, "Error: Вы выбрали режим орнамента круг, но добавили лишний аргумент --count!\n");
                return 1;
            }

            break;
        case semicircle:
            if (checker_color(figure)) return 1;
            if (checker_thinckness(figure)) return 1;
            if (figure->count == -1) {
                fprintf(stderr, "Error: Вы не ввели количество!\n");
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
bool check_rotate(object_t* figure) {
    if (checker_left_up(figure)) return 1;

    if (checker_right_down(figure)) return 1;

    if (figure->angle == -1 ) {
        fprintf(stderr, "Error: Вы не выбрали угол поворота!\n");
        return 1;
    }

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
bool base_checker(object_t* figure) {
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
            break;
        default:
            fprintf(stderr, "Error: Невозможная ошибка!\n");
            return 1;
    }

    return res_check;
}

int main(int argc, char* argv[]){
    object_t* figure = malloc(sizeof(object_t));
    constuctor(figure);


    if (base_parser(figure, argc, argv)) return 41;

    if (base_checker(figure)) return 42;

    printf("mod %d\n", figure->mod);
    printf("pattern %d\n", figure->pattern);
    printf(" x_lu %d\n", figure->x_left_up);
    printf("y_lu %d\n", figure->y_left_up);
    printf("x_rd %d\n", figure->x_right_down);
    printf("y_rd %d\n", figure->y_right_down);
    printf("thincless %d\n", figure->thinckness);
    printf("color %d  %d  %d\n", figure->color_r, figure->color_g, figure->color_b);
    printf("fill %d\n", figure->fill);
    printf("color_fill %d  %d  %d\n", figure->color_fill_r, figure->color_fill_g, figure->color_fill_b);
    printf("count %d\n", figure->count);
    printf("angle  %d\n", figure->angle);
    printf("x_c, y_c %d  %d\n", figure->x_center, figure->y_center);
    printf("rad %d\n", figure->radius);



    puts(figure->start_filename);

    puts(figure->finish_filename);


    rotate_area(*figure);
    free(figure);

    return 0;
}


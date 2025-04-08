/**
* @file structures.h
 * @brief Файл, где содержатся основные структуры и инклуды.
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

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


typedef struct {
    int thickness;
    point_t point_2;
    point_t point_1;
} line_t;


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
void constuctor(object_t*);

#endif // STRUCTURES_H

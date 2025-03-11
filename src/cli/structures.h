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

/**
 * @brief Перечисление режимов для орнамента.
 */
typedef enum Pattern {rectangle, circle, semicircle, none} pattern_t;

/**
 * @brief Перечисление режимов допустимых операций.
 */
typedef enum Mode {rect, ornament, rotate, circ, info, help, None} mode;

/**
 * @brief Данное перечисление является хранилищем режимов для функции comp_cnt_args.
 */
enum count_arguments {one_read = 1, two_read = 2, three_read = 3};


/**
 * @brief Структура, которая является собирательным образом всех парметров, которые нужны для 
 * работы каждого из режимов. Предполагается, что взаимодействие внутри программы происходит
 * за счёт последовательной передачи объекта данной структуры.
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
 * @brief Функция конструктор заполняет структуру базовыми значениями, предполагается, что такие значения не могут
 * получится в случае парсера, поэтому можно будет проверить, ввел ли пользователь тому или иному полю значение.
 * @param obj Объект, который нужно инициализировать.
 */
void constuctor(object_t*);

#endif // STRUCTURES_H

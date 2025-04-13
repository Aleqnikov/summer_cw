/**
* @file help_canvas_fns.c
 * @brief  Содержит вспомогательные функции для рисования фигур на изображении.
 * 
 * Используется в файлах canvas_[...].
 */

#include "help_canvas_fns.h"

/**
 * @brief Данная функция проверяет, выходит ли за пределы изображения координаты.
 * 
 * Используется при рисовании различных фигур.
 * 
 * @param Получает координаты точки, и характеристики изображения.
 */
bool check_coord(int y, int x, int height, int weight){
    if((x >= 0 && x < weight) && (y >= 0 && y < height))
        return 1;
    return 0;
}

/**
 * @brief Данная функция копирует переданный ей массив типа Rgb и затем возварщает ссылку на новый скопированный массив.
 * 
 * Используется в функции поворота части изображения, для того чтобы получить новый массив, и поместить на 
 * него новые пиксели, перемещённой области.
 * 
 * @param Массив данных, который нужно скопировать, и его хараектеристики, высота и длина.
 */
Rgb** copy_array(Rgb** data, int height, int width){
    Rgb** new_array = NULL;
    new_array = (Rgb** )(malloc(sizeof(Rgb* ) * (height)));

    if (new_array == NULL)
        return NULL;

    for (int i = 0; i < (height); i++) {
        (new_array)[i] = (Rgb *)malloc(width * sizeof(Rgb));
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++)
            new_array[i][j] = data[i][j];
    }

    return new_array;
}

/**
* @brief Функция синуса, в зависимости от переданного угла возвращает значение.

* Используется в функции поворота части изображения, для применения аффинных преобразований.

* @param Получает угол 90 180 или 270 градусов. В ином случае не делает ничего, ибо другого угла передано быть не может.
*/
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

/**
* @brief Функция синуса, в зависимости от переданного угла возвращает значение.

* Используется в функции поворота части изображения, для применения аффинных преобразований.

* @param Получает угол 90 180 или 270 градусов. В ином случае не делает ничего, ибо другого угла передано быть не может.
*/
int custom_sin(int angle) {
    switch (angle) {
        case 90:
            return -1;
        case 180:
            return 0;
        case 270:
            return 1;
    }
}

/**
 * @brief Данная функция проверяет входит ли функция в диапозон.
 * 
 * Используется в функции rotate.
 * @param value Значение, которуе нужно проверить.
 * @param low Нижняя грань.
 * @param high Верхнаяя грань.
 * @param thinkless Значение толщины линии.
 */
int get_value_range(int value, int low, int high, int thinkless){

    if(value >= low - thinkless && value <= high + thinkless)
        return value;
    if(value > high + thinkless) return high + thinkless;
    if(value < low - thinkless) return low - 1 - thinkless;
}

int max(int x, int y){
    return x >= y? x: y;
}

int min (int x, int y){
    return x <= y ? x : y;
}
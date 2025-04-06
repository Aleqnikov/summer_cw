/**
* @file main.с
 * @brief Основной файл программы.
 */

#include "structures.h"
#include "parcer/parcer.h"
#include "checker/base_checker.h"
#include "Image/canvas.h"



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

int main(int argc, char* argv[]){

    printf("Course work for option 5.1, created by Ivan Aleinikov\n");
    object_t* figure = malloc(sizeof(object_t));
    constuctor(figure);

    if (base_parser(figure, argc, argv)) return 41;

    if (base_checker(figure)) return 42;

    canvas_logic(*figure);

    free(figure);

    return 0;
}


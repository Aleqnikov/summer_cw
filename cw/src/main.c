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

int main(int argc, char* argv[]){
    printf("Course work for option 5.1, created by Ivan Aleinikov\n");

    object_t* figure = malloc(sizeof(object_t));
    constuctor(figure);

    switch (base_parser(figure, argc, argv)) {
        case 1:
            return 41;
        case 2:
            return 0;
    }
        
    switch (base_checker(figure)) {
        case 1:
            help_print();
            return 42;
        case 2:
            fprintf(stderr, "Error: были переданны неккоректные параметры для выборанной вами функци!\n");
            help_print();
            return 43;
    }

    canvas_logic(*figure);

    free(figure);

    return 0;
}
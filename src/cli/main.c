/**
* @file main.с
 * @brief Основной файл программы.
 */

#include "structures.h"
#include "parcer/parcer.h"
#include "checker/base_checker.h"


/**
 * @brief Функция конструктор заполняет структуру базовыми значениями, предполагается, что такие значения не могут
 * получится в случае парсера, поэтому можно будет проверить, ввел ли пользователь тому или иному полю значение.
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
    free(figure);

    return 0;
}

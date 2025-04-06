/**
* @file check_functions.c
 * @brief  Содержит  функции для проверки данных.
 */


#include "check_functions.h"

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

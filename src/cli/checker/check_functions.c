/**
* @file check_functions.c
 * @brief  Содержит  функции для проверки данных.
 */


#include "check_functions.h"

/**
 * @brief Данная функция проверяет корректность заполнености структуры для режима работы --rect.
 * @param figure Указатель на структуру.
 * @return Ноль или единица
 */
int check_rectangle(object_t* figure) {
    if (checker_left_up(figure)) return 1;

    if (checker_right_down(figure)) return 1;

    if (checker_thinckness(figure)) return 1;

    if (checker_color(figure)) return 1;

    if (checker_fill(figure)) return 1;

    return 0;
}

/**
 * @brief Данная функция проверяет корректность заполнености структуры для режима работы --circle.
 * @param figure Указатель на структуру.
 * @return Ноль или единица
 */
int check_circle(object_t* figure) {
    // Проверяем специфические параметры
    if (figure->x_center == -1 || figure->y_center == -1) {
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
 * @brief Проверят корректность полей для ornament.
 * @param figure Указатель на объект.
 * @return Ноль в случае удачи в ином случае единица.
 */
int check_ornament(object_t* figure) {

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
        case circle: // У него только цвет
            break;
        case semicircle:
            if (checker_color(figure)) return 1;
            if (checker_thinckness(figure)) return 1;
            if (figure->count == -1) {
                fprintf(stderr, "Error: Вы не ввели количество!\n");
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Error: Невозможная ошибка в проверка орнамента.\n");
            return 1
        ;
    }

    return 0;
}

/**
 * @brief Проверят корректность полей для ornament.
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

    return 0;
}
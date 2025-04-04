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
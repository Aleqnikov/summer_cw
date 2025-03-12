/**
* @file help_functions.c
 * @brief  Содержит вспомогательные функции для проверки данных.
 */

#include "help_functions.h"

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
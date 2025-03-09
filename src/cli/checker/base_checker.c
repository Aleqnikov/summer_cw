/**
* @file base_checker.c
 * @brief  Содержит главную функцию для проверки данных.
 */

#include "base_checker.h"

/**
 * @brief Данная функция является проверкой корректности переданнх значений, соответствие режимам и т.п.
 * Простые проверки производит сама, но более ветвистые перераспределяет по зависимым вспомогательным функциям.
 * @param figure Указатель на проверяемый объект
 * @return Если все проверки прошли, то ноль, в ином случае код ошибки.
 */
int base_checker(object_t* figure) {
    if (figure->start_filename == NULL) {
        fprintf(stderr, "Error: Не было введено имя файла!\n");
        return 1;
    }

    if (strcmp(figure->start_filename, figure->finish_filename) == 0) {
        fprintf(stderr, "Error: Начальные и стартовые имена совпадают!\n");
        return 1;
    }

    if (check_dt_bmp(figure->start_filename)) {
        fprintf(stderr, "Error: Изначальный файл не имеет расширение .bmp!\n");
        return 1;
    }

    if (check_dt_bmp(figure->finish_filename)) {
        fprintf(stderr, "Error: Конечный файл не имеет расширение .bmp!\n");
        return 1;
    }

    int res_check = 0;
    switch (figure->mod) {
        case rect:
            res_check = check_rectangle(figure);
        break;
        case ornament:
            res_check = check_ornament(figure);
        break;
        case rotate:
            res_check = check_rotate(figure);
        break;
        case circ:
            res_check = check_circle(figure);
        break;
        default:
            fprintf(stderr, "Error: Невозможная ошибка!\n");
        return 1;
    }

    return res_check;
}
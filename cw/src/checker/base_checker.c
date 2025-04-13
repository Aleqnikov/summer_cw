/**
* @file base_checker.c
 * @brief  Содержит главную функцию для проверки данных.
 */

#include "base_checker.h"

/**
 * @brief Данная функция является проверкой корректности переданнх значений, соответствие режимам и т.п.
 * Простые проверки производит сама, но более ветвистые перераспределяет по зависимым вспомогательным функциям.
 *
 * Данная функция является главной функцией для проверки корректности заполенности полей переданного экземпляра
 * структуры. Проверят пустое ли поле названия входного файла. На совпадение названия начального и конечного названия.
 * проверят на корректность расширения.
 * Используется как функция проверки заполненности структуры.
 *
 * @param figure Указатель на проверяемый объект
 * @return Если все проверки прошли, то ноль, в ином случае код ошибки.x
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
        case info:  
            res_check = check_info(figure);
            break;
        case help:
            break;
        default:
            fprintf(stderr, "Error: Не выбран режим работы!\n");
            return 1;
    }

    return res_check;
}
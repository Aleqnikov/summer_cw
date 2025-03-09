/**
* @file ornament.c
 * @brief  Содержит функции для парсинга режима орнамент.
 */

#include "ornament.h"

/**
 * @brief Данная функция является парсером для орнамента, режима rect, semicircles.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
int ornament_rect_semicrcls(object_t* figure, int argc, char** argv) {
    static struct option long_options[] = {
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'C'},
        {"count", required_argument, 0, 'c'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:C:c:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 't': {
                int result_t = thickness(figure, argc, argv);
                if (result_t != 0) return result_t;
                break;
            }
            case 'C': {
                int result_C = color(figure, argc, argv);
                if (result_C != 0) return result_C;
                break;
            }
            case 'c': {
                int result_c = count(figure, argc, argv);
                if (result_c != 0) return result_c;
                break;
            }
            case 'i': {
                int res_i = input_name(figure, argc, argv);
                if (res_i != 0) return res_i;
                break;
            }
            case 'o': {
                int res_o = output_name(figure, argc, argv);
                if (res_o != 0) return res_o;
                break;
            }
            case '?':
                fprintf(stderr, "Error: Некорректный аргумент.\n");
                return -1;
        }
    }

    get_filename(figure, argc, argv);

    return 0;
}

/**
 * @brief Данная функция является парсером для орнамента, режима circle.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
int ornament_circle(object_t* figure, int argc, char** argv) {
    static struct option long_options[] = {
        {"color", required_argument, 0, 'C'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "C:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'C': {
                int result_C = color(figure, argc, argv);
                if (result_C != 0) return result_C;
                break;
            }
            case 'i': {
                int res_i = input_name(figure, argc, argv);
                if (res_i != 0) return res_i;
                break;
            }
            case 'o': {
                int res_o = output_name(figure, argc, argv);
                if (res_o != 0) return res_o;
                break;
            }
            case '?':
                fprintf(stderr, "Ошибка: некорректный аргумент.\n");
            return -1;
        }
    }

    get_filename(figure, argc, argv);

    return 0;
}


/**
 * @brief Данная функция является парсером для --ornament.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
int parce_ornament(int argc, char** argv, object_t* figure) {
    int count_args = is_correct_count_args(argc, argv, "--ornament");
    if (count_args != 0) return count_args;

    int is_word = -1;

    if (strcmp(optarg, "rectangle") == 0) is_word = 0;
    if (strcmp(optarg, "circle") == 0) is_word = 1;
    if (strcmp(optarg, "semicircles") == 0) is_word = 2;

    switch (is_word) {
        case rectangle: {
            figure->pattern = rectangle;
            int res_r = ornament_rect_semicrcls(figure, argc, argv);
            if (res_r != 0) return res_r;
            break;
        }
        case circle: {
            figure->pattern = circle;
            int res_c = ornament_circle(figure, argc, argv);
            if (res_c != 0) return res_c;
            break;
        }
        case semicircle: {
            figure->pattern = semicircle;
            int res_s = ornament_rect_semicrcls(figure, argc, argv);
            if (res_s != 0) return res_s;
            break;
        }
        default:
            fprintf(stderr, "Ошибка: вы выбрали неправильный режим орнамента!\n");
        return 1;
    }

    return 0;
}
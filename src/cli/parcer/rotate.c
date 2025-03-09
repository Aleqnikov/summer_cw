/**
* @file rotate.c
 * @brief  Содержит функции для парсинга режима rotate.
 */

#include "rotate.h"

/**
 * @brief Данная функция является парсером для --rect.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
int parce_rotate(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"angle", required_argument, 0, 'a'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "l:r:a:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'l': {
                int result_l = left_up(figure, argc, argv);
                if (result_l != 0) return result_l;
                break;
            }
            case 'r': {
                int result_r = right_down(figure, argc, argv);
                if (result_r != 0) return result_r;
                break;
            }
            case 'a': {
                int result_a = angle(figure, argc, argv);
                if (result_a != 0) return result_a;
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

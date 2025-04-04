/**
* @file rectangle.c
 * @brief  Содержит функции для парсинга режима rectangle.
 */

#include "rectangle.h"


/**
 * @brief Данная функция является парсером --rect.
 *
 * Данная функция является парсером для режима --rect, она считывает переданные после флаги, и посредством ветвления
 * распределяет задачи по вспомогательным функциям.
 * Используется при парсинге режима --rect.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool parce_rectangle(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'c'},
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},

        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "l:r:t:c:fF:i:o:", long_options, &option_index)) != -1) {

        switch (opt) {
            case 'l': {
                if (left_up(figure, argc, argv))
                    return 1;
                break;
            }
            case 'r': {
                if (right_down(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 't': {
                if (thickness(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'c': {
                if (color(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'f':
                if (optind < argc && argv[optind][0] != '-' && !(isalpha(argv[optind][0]) && optind  + 1 == argc)) {
                    fprintf(stderr, "Error: --fill не принимает аргументы!\n");
                    return -1;
                }
                figure->fill = true;
                break;
            case 'F':
                if (fill_color(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'i':
                if (input_name(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'o':
                if (output_name(figure, argc, argv) != 0)
                    return 1;
                break;

            case '?':
                fprintf(stderr, "Error: некорректный аргумент.\n");
                return 1;
        }
    }

    get_filename(figure, argc, argv);
    return 0;
}
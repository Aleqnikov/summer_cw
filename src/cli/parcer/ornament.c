/**
* @file ornament.c
 * @brief  Содержит функции для парсинга режима орнамент.
 */

#include "ornament.h"

/**
 * @brief Данная функция является парсером для --ornament.
 *
 * Данная функция является парсером для режима --ornament, она считывает переданные после флаги, и посредством ветвления
 * распределяет задачи по вспомогательным функциям.
 * Используется при парсинге режима --ornament.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool parce_ornament(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"pattern", required_argument, 0, 'p'},
        {"count", required_argument, 0, 'c'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'C'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "p:c:t:C:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'p': {
                int count_args = is_correct_count_args(argc, argv, "--pattern", figure->start_filename);
                if (count_args != 0) return count_args;

                if (!strcmp(optarg, "rectangle")) figure->pattern = rectangle;
                if (!strcmp(optarg, "semicircles")) figure->pattern = semicircle;
                if (!strcmp(optarg, "circle")) figure->pattern = circle;

                if (figure->pattern == none) {
                    fprintf(stderr, "Error: Вы не выбрали режим орнамента!\n");
                    return 1;
                }
                break;

            }
            case 'c': {
                if (count(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 't': {
                if (thickness(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'C': {
                if (color(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'i': {
                if (input_name(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case 'o': {
                if (output_name(figure, argc, argv) != 0)
                    return 1;
                break;
            }
            case '?':
                fprintf(stderr, "Error: некорректный аргумент.\n");
                return 1;
        }
    }

    get_filename(figure, argc, argv);
    return 0;
}
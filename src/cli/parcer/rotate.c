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
bool parce_rotate(int argc, char** argv, object_t* figure) {
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
            case 'l':
                if (left_up(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'r':
                if (right_down(figure, argc, argv) != 0)
                    return 1;
                break;

            case 'a':
                if (angle(figure, argc, argv) != 0)
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

/**
* @file parcer.c
 * @brief  Содержит главные функции для парсинга.
 */

#include "parcer.h"

// Пока так.
void print_help() {
    printf("Course work for option 5.1, created by Ivan Aleinikov\n");
}

/**
 * @brief Данная функция является логической функции парсера, которая проверяет все команды
 * и передаёт их в зависимые функции.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
int base_parser(object_t* figure, int argc, char** argv) {
    static struct option long_options[] = {
        {"rect", no_argument, 0, 0},
        {"ornament", required_argument, 0, 0},
        {"rotate", no_argument, 0, 0},
        {"circle", no_argument, 0, 0},
        {"info", required_argument, 0, 0},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;
    opt = getopt_long(argc, argv, "h", long_options, &option_index);

    if (opt == 'h') {
        print_help();
        return 1;
    }

    if (opt == 0) {
        int res = 0;
        switch (option_index) {
            case rect:
                figure->mod = rect;
                res = parce_rectangle(argc, argv, figure);
                break;
            case ornament:
                figure->mod = ornament;
                res = parce_ornament(argc, argv, figure);
                break;
            case rotate:
                figure->mod = rotate;
                res = parce_rotate(argc, argv, figure);
                break;
            case circ:
                figure->mod = circ;
                res = parce_circle(argc, argv, figure);
                break;
            case info:
                figure->mod = info;
                if(optarg){
                    figure->start_filename = optarg;
                    res = 0;
                }

                fprintf(stderr, "Error: не было передано название файла!\n");
                return 1;

            case help:
                print_help();
                return 69;

            case '?':
                fprintf(stderr, "Error: была переданна неизвестная коменда!\n");
                return 1;
        }
        return res;
    }

    return 0;
}
/**
* @file parcer.c
 * @brief  Содержит главные функции для парсинга.
 */

#include "parcer.h"


/**
 * @brief Данная функция является логической функции парсера, которая проверяет все команды
 * и передаёт их в зависимые функции.
 *
 * Данная функция является главной логической функцией, которая занимается парсингом всех переданных аргументов в
 * программу. Она представляет собой инициализированный список флагов, а затем ветвистую стркутуру, которая
 * в зависимости от считанного режима запускает выполнение того или иного флага.
 * Используется в программе для парсинга аргументов.
 *
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
bool base_parser(object_t* figure, int argc, char** argv) {

    if (argc == 1){
        help_print();
        return 0;
      }


    static struct option long_options[] = {
        {"rect", no_argument, 0, 0},
        {"ornament", no_argument, 0, 0},
        {"rotate", no_argument, 0, 0},
        {"circle", no_argument, 0, 0},
        {"info", required_argument, 0, 0},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;
    opt = getopt_long(argc, argv, "h", long_options, &option_index);

    if (opt == 'h') {
        if (argc != 2) {            // Проверяем, что количество аргументов равно единице.
            fprintf(stderr, "Error: Передано слишком много аргументов.\n");
            return 1;
        }

        help_print();
        return 1;
    }

    if (opt == 0) {
        int res = 0;
        switch (option_index) {
            case rect:

                if (no_arguments(argc, argv, "--rect"))
                    return 1;

                figure->mod = rect;
                res = parce_rectangle(argc, argv, figure);
                break;
            case ornament:
                if (no_arguments(argc, argv, "--ornament"))
                    return 1;

                figure->mod = ornament;
                res = parce_ornament(argc, argv, figure);
                break;
            case rotate:
                if (no_arguments(argc, argv, "--rotate"))
                    return 1;
                figure->mod = rotate;
                res = parce_rotate(argc, argv, figure);
                break;
            case circ:
                if (no_arguments(argc, argv, "--circle"))
                    return 1;
                figure->mod = circ;
                res = parce_circle(argc, argv, figure);
                break;
            case info:
                figure->mod = info;

                if (argc != 3) {        // В данном случае, проверяем, что передано всего два аргумента.
                    fprintf(stderr, "Error: Передано слишком много аргументов.\n");
                    return 1;
                }

                if(!optarg){
                    fprintf(stderr, "Error: не было передано название файла!\n");
                    return 1;
                }

                figure->start_filename = optarg;
                break;

            case help:

                if (argc != 2) {            // Проверяем, что количество аргументов равно единице.
                    fprintf(stderr, "Error: Передано слишком много аргументов.\n");
                    return 1;
                }
                help_print();
                return 1;

            case '?':
                fprintf(stderr, "Error: была переданна неизвестная коменда!\n");
                return 1;
        }
        return res;
    }

    return 0;
}


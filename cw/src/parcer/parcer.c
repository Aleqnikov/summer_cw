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
int base_parser(object_t* figure, int argc, char** argv) {

    if (argc == 1){
        help_print();
        return 1;
    }

    static struct option long_options[] = {
        {"rect", no_argument, 0, 0},
        {"ornament", no_argument, 0, 0},
        {"rotate", no_argument, 0, 0},
        {"circle", no_argument, 0, 0},
        {"info", no_argument, 0, 0},
        {"help", no_argument, 0, 'h'},
        {"pattern", required_argument, 0, 'p'},
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"thickness", required_argument, 0, 't'},
        {"output", required_argument, 0, 'o'},
        {"input", required_argument, 0, 'i'},
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {"angle", required_argument, 0, 'a'},
        {"color", required_argument, 0, 'c'},
        {"count", required_argument, 0, 'C'},
        {"center", required_argument, 0, 'n'},
        {"radius", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "hp:l:r:t:o:i:fF:a:c:C:n:R:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 0:
                if(set_mode(long_options[option_index].name, figure)){
                    fprintf(stderr, "Error: Программа за один запуск может выполнить только одну функцию!\n");
                    return 1;
                }
                break;

            case 'h':
                help_print();
                return 2;

            case 'p':
                if (parce_pattern(optarg, figure)) return 1;
                break;

            case 'l':
                if (parce_left_up(optarg, figure))
                    return 1;
                break;

            case 'r':
                if (parce_right_down(optarg, figure))
                    return 1;
                break;

            case 'a':
                if (parce_angle(optarg, figure))
                    return 1;
                break;

            case 'i':
                figure->start_filename = optarg;
                break;

            case 'o':
                figure->finish_filename = optarg;
                break;

            case 'C': {
                if (parce_count(optarg, figure))
                    return 1;
                break;
            }
            case 't': {
                if (parce_thickness(optarg, figure))
                    return 1;
                break;
            }
            case 'c': {
                if (parce_color(optarg, figure))
                    return 1;
                break;
            }

            case 'f':
                figure->fill = true;
                break;
            case 'F':
                if (parce_fill_color(optarg, figure))
                    return 1;
                
                break;

            case 'n':
                if (parce_center(optarg, figure))
                    return 1;
                break;

            case 'R':
                if (parce_radius(optarg, figure))
                    return 1;
                break;

            case '?':
                fprintf(stderr, "Error: Вы ввели неккоректную опцию!\n");
                return 1;

            default:
                fprintf(stderr, "Error: Ошибка парсинга аргументов!\n");
                return 1;
        }
    }

    if (optind + 1 == argc && figure->start_filename == NULL) {
        figure->start_filename = argv[optind];
        return 0;
    }


    if (optind  < argc){
        fprintf(stderr, "Error: Введены лишние аргументы!\n");
        while (optind < argc)
            fprintf(stderr, "Error: лишний аргумент - %s\n", argv[optind++]);
        return 1;
    }

    return 0;
}
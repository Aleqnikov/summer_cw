#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/** 
 * @file main.с
 * @brief Весенняя курсовая работа
 */


/**
 * @brief Перечисление режимов для орнамента.
 */
typedef enum Pattern {rectangle, circle, semicircle, none} pattern_t;

/**
 * @brief Перечисление режимов допустимых операций.
 */
typedef enum Mode {rect, ornament, rotate, circ, info, help, None} mode;


/**
 * @brief Структура, которая является собирательным образом всех парметров, которые нужны для 
 * работы каждого из режимов. Предполагается, что взаимодействие внутри программы происходит
 * за счёт последовательной передачи объекта данной структуры.
 */

typedef struct Object {
    /** @brief Режим работы программы.*/
    mode mod;  

    char* start_filename;                               /** @brief Имя файла, который нужно обработать.*/
    char* finish_filename;                              /** @brief Имя файла, который должно быть после обработки.*/

    int x_left_up, y_left_up;                           /** @brief Координаты левой верхней точки.*/
    int x_right_down, y_right_down;                     /** @brief Координаты правой нижней точки.*/

    int thinckness;                                     /** @brief Толшина линии.*/

    int color_r, color_g, color_b;                      /** @brief Цвет линии.*/

    bool fill;                                          /** @brief Нужно ли заливать фигуру.*/

    int color_fill_r, color_fill_g, color_fill_b;       /** @brief Цвета заливки*/

    pattern_t pattern;                                  /** @brief Режим для создания узора.*/

    int count;                                          /** @brief Количество элементов узора.*/

    int angle;                                          /** @brief Угол поворота части изображения.*/

    int x_center, y_center;                             /** @brief Координаты центра окружности.*/
    int radius;                                         /** @brief Радиус окружности*/
} object_t ;

/**
 * @brief Функция конструктор заполняет структуру базовыми значениями, предполагается, что такие значения не могут
 * получится в случае парсера, поэтому можно будет проверить, ввел ли пользователь тому или иному полю значение.
 * @param obj Объект, который нужно инициализировать.
 */
void constuctor(object_t* obj) {
    obj->mod = None;
    obj->start_filename = NULL;
    obj->finish_filename = "out.bmp";
    obj->x_left_up = -1;
    obj->y_left_up = -1;
    obj->x_right_down = -1;
    obj->y_right_down = -1;
    obj->pattern = none;
    obj->count = -1;
    obj->angle = -1;
    obj->x_center = -1;
    obj->y_center = -1;
    obj->radius = -1;
    obj->thinckness = -1;
    obj->fill = false;
    obj->color_fill_b = -1;
    obj->color_fill_r = -1;
    obj->color_fill_g = -1;
    obj->color_r = -1;
    obj->color_g = -1;
    obj->color_b = -1;
}

void help_print(void) {
    printf("╭━━╮╭━╮╭━┳━━━╮╭━━━┳━━━┳━━┳━━━━┳━━━┳━━━╮\n"
            "┃╭╮┃┃┃╰╯┃┃╭━╮┃┃╭━━┻╮╭╮┣┫┣┫╭╮╭╮┃╭━╮┃╭━╮┃\n"
            "┃╰╯╰┫╭╮╭╮┃╰━╯┃┃╰━━╮┃┃┃┃┃┃╰╯┃┃╰┫┃╱┃┃╰━╯┃\n"
            "┃╭━╮┃┃┃┃┃┃╭━━╯┃╭━━╯┃┃┃┃┃┃╱╱┃┃╱┃┃╱┃┃╭╮╭╯\n"
            "┃╰━╯┃┃┃┃┃┃┃╱╱╱┃╰━━┳╯╰╯┣┫┣╮╱┃┃╱┃╰━╯┃┃┃╰╮\n"
            "╰━━━┻╯╰╯╰┻╯╱╱╱╰━━━┻━━━┻━━╯╱╰╯╱╰━━━┻╯╰━╯\n");
    printf("\n\n Справка о программе!\n");

    printf("Данная программа обладает флагами:\n"
           "    -h, --help                                                  Печатает информационную справку.\n"
           "    --info <file.bmp>                                       Печатает информацию о bmp файле.\n\n"
           "    --rect  [options]\n"
           "        -l --left_up <x.y>                                      Устанавливает координаты левой точки.\n"
           "        -r --right_down <x.y>                                   Устанавливает координаты правой нижней точки.\n"
           "        -t --thickness <num>                                    Устанавливает толщину линий.\n"
           "        -с --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
           "        -f --fill                                               Устанавливает нужна ли заливка.\n"
           "        -F --fill_color <rrr.ggg.bbb>                           Устанавливает цвета заливки.\n\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --ornament [options] \n"
           "        -p --pattern [rectangle, circle, cemicircle]            Устанавливает режим орнамента.\n"
           "        -t --thickness <num>                                    Устанавливает тольщину линии.\n"
           "        -C --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
           "        -c --count <num>                                        Устанавливает количество.\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --rotate [options] \n"
           "        -l --left_up <x.y>                                      Устанавливает координаты левой точки.\n"
           "        -r --right_down <x.y>                                   Устанавливает координаты правой нижней точки.\n"
           "        -a --angle <num>                                        Устанавливает угол, может быть 90, 180, 270\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n"
           "    --circle [options]\n"
           "        -c --center <x.y>                                       Устанавливает центр окружности.\n"
           "        -r --radius <num>                                       Устанавливает радиус окружности.\n"
           "        -t --thickness <num>                                    Устанавливает толщину линий.\n"
           "        -С --color <rrr.ggg.bbb>                                Устанавливет цвет линии.\n"
           "        -f --fill                                               Устанавливает нужна ли заливка.\n"
           "        -F --fill_color <rrr.ggg.bbb>                           Устанавливает цвета заливки.\n\n"
           "        -o --output <file.bmp>                                  Устанавливает название выходного файла.\n"
           "        -i --input <file.bmp>                                   Устанавливает название входного файла.\n");
}


/**
 * @brief Данная функция проверяет корректность аргументов, проверяет что строку полностью состоит из чисел
 * @param str строка, которую нужно проверить
 * @return Если есть что то, кроме чисел, то возвращает ноль, иначе единицу.
 */
bool is_number(const char *str) {
    if (!str || *str == '\0') return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-') return 0;
        str++;
    }
    return 1;
}

/**
 * @brief Данная функция проверяет корректность аргументов парсера, которые заданы через точку (x.y).
 * @param str Строка, содержимое которое необходимо проверить на корректность.
 * @param count_dots Количество точек, которое должно быть в строке.
 * @return
 */
bool is_correct_dots(const char *str, int count_dots) {
    char* dot = strchr(str, '.');

    for (int i = 0; i < count_dots; i++) {
        if (!dot) return 0;   // Если точек меньше, то завершаем программу.
        dot = strchr(dot + 1, '.');
    }

    // Проверка на корректность символов.
    if (!str || *str == '\0')
        return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-' && *str != '.') return 0;
        str++;
    }

    return !(dot == NULL);
}

/**
 * @brief Данная функция проверят есть ли лишний аргумент после функции.
 * @param argc Количество аргументов.
 * @param argv Массив указателей на аргументы.
 * @param name Имя функции, где произошла проверка.
 * @return Единицу, если слишком много аргументов, ноль, если все в норме.
 */
bool is_correct_count_args(int argc, char** argv, char* name) {

    if (!optarg) {
        fprintf(stderr, "Error: Не был передан аргумент для %s!\n", name);
        return 1;
    }

    if (optarg[0] == '-') {
        fprintf(stderr, "Error: вы не ввели аргументы для функции %s!\n", name);
        return 1;
    }

    if ( optind < argc && argv[optind][0] != '-' && !(isalpha(argv[optind][0]) && optind  + 1 == argc)) {
        fprintf(stderr, "Ошибка: вы ввели слишком много аргументов для %s\n", name);
        return 1;
    }

    return 0;
}

/**
 * @brief Проверяет координаты на корректность, в данном случае количество, и нахождение в первой четверти.
 * @param x Координата по x.
 * @param y Координаты по y.
 * @param count Количество считанных координат.
 * @return Булевое значение.
 */
bool is_correct_coords(int x, int y, int count) {
    if (count != 2) {
        fprintf(stderr, "Error: Неккоректные координаты! Их должно быть два числовых значения!\n");
        return 1;
    }


    if (x < 0 || y < 0) {
        fprintf(stderr, "Error: Неккоректные координаты! Должны быть больше нуля!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция осуществляет проверку на корректность аргументов - цвета.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @param name Название флага.
 * @return Если все правильно, то 0, иначе код ошибки.
 */
int check_color(int argc, char** argv, char* name) {
    int count_args = is_correct_count_args(argc, argv, name);
    if (count_args != 0) return count_args;

    if (is_correct_dots(optarg, 2)) {
        fprintf(stderr, "Error: Цвета переданы в некорректном формате! Должно быть - r.g.b\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Вспомогательная функция, помогает определить корректность цвета, диапазон, и количество считанного.
 * @param count Количество прочитанных аргументов.
 * @param red Число красного цвета.
 * @param green Число зелёного цвета.
 * @param blue Число синего цвета.
 * @return
 */
int check_colors(int count, int red, int green, int blue) {
    if (count != 3) {
        fprintf(stderr, "Error: передано слишком мало аргументов для цвета.\n");
        return 1;
    }

    if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
        fprintf(stderr, "Error: цвета переданы в неккоректном формате! Каждый должен быть в диапазоне [0, 255]");
        return 1;
    }

    return 0;
}

/**
 *@brief Данная функция проверят в правильном ли формате переданы координаты точки.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @param name Имя проверяемого флага.
 * @return Ноль, если все в порядке, единицу в противном случае.
 */
int parce_coords(int argc, char** argv, char* name) {
    // Проверка на корректность количества аргументов.
    int count_args = is_correct_count_args(argc, argv, name);
    if (count_args != 0) return count_args;

    // Вторая проверка, на количество точек, гарантирует, что передано правильно количество аргументов.
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция является реализацией парсера флага --left_up.
 * @param figure Указатель на объект фигуры.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Возвращает ноль, если все в порядке, в ином случае либо код ошибки, либо единицу.
 */
int left_up(object_t* figure, int argc, char** argv) {

    int result_coords = parce_coords(argc, argv, "left_up");
    if (result_coords != 0)
        return result_coords;

    int x, y;
    int res = sscanf(optarg, "%d.%d", &x, &y);

    if (is_correct_coords(x, y, res))
        return 1;

    figure->x_left_up = x; figure->y_left_up = y;

    return 0;
}

/**
 * @brief Данная функция является реализацией парсера флага --right_down.
 * @param figure Указатель на объект фигуры.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Возвращает ноль, если все в порядке, в ином случае либо код ошибки, либо единицу.
 */
int right_down(object_t* figure, int argc, char** argv) {

    int result_coords = parce_coords(argc, argv, "right_down");
    if (result_coords != 0)
        return result_coords;
    int x, y;
    int res = sscanf(optarg, "%d.%d", &x, &y);

    if (is_correct_coords(x, y, res))
        return 1;

    figure->x_right_down = x; figure->y_right_down = y;
    return 0;
}

/**
 * @brief Данная функция явяется реализацией парсера для флага --thickness.
 * @param figure Указатель на объект данных.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль, если все в порядке, в ином случает -1 если возникла ошибка, или иной код ошибки.
 */
int thickness(object_t* figure, int argc, char** argv) {

    int count_args = is_correct_count_args(argc, argv, "--thickness");
    if (count_args != 0) return count_args;

    if (!is_number(optarg)) {
        fprintf(stderr, "Ошибка: вы ввели  не число для --thickness\n");
        return -1;
    }

    int res = sscanf(optarg, "%d", &figure->thinckness);

    if (res != 1) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для --thickness\n");
        return -1;
    }

    if (figure->thinckness <= 0) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для, должны быть больше нуля! --thickness\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Функция, которая реализует парсинг флага --color.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Если все в порядке, то ноль, в ином случае код ошибки.
 */
int color(object_t* figure, int argc, char** argv) {
    int first_check = check_color(argc, argv, "--color");
    if (first_check != 0) return first_check;

    int red, green, blue;
    int res = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    int second_check = check_colors(res, red, green, blue);
    if (second_check != 0) return second_check;

    figure->color_r = red; figure->color_g = green; figure->color_b = blue;
    return 0;
}

/**
 * @brief Функция, которая реализует парсинг флага --fill_color.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Если все в порядке, то ноль, в ином случае код ошибки.
 */
int fill_color(object_t* figure, int argc, char** argv) {
    int first_check = check_color(argc, argv, "--fill_color");
    if (first_check != 0) return first_check;

    int red, green, blue;
    int res = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    int second_check = check_colors(res, red, green, blue);
    if (second_check != 0) return second_check;

    figure->color_fill_r = red; figure->color_fill_g = green; figure->color_fill_b = blue;
    return 0;
}

/**
 * @brief Функция, которая реализиет парсинг флага --input.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль, если все хорошо, в ином случае единицу.
 */
int input_name(object_t* figure, int argc, char** argv) {
    int check_args = is_correct_count_args(argc, argv, "--input");
    if (check_args != 0) return check_args;

    figure->start_filename = optarg;
    return 0;
}

/**
 * @brief Функция, которая реализует парсинг флага --output.
 * @param figure Указатель на фигуру.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Если все нормально ноль, в ином случае код ошибки.
 */
int output_name(object_t* figure, int argc, char** argv) {
    int check_args = is_correct_count_args(argc, argv, "output_name");
    if (check_args != 0) return check_args;

    figure->finish_filename = optarg;
    return 0;
}

/**
 * @brief Функция для получения имени входного файла из конца.
 * @param figure Указатель на фигуру.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 */
void get_filename(object_t* figure, int argc, char** argv) {
    if (figure->start_filename == NULL && ((argc - 2 > 0 && argv[argc - 2][0] != '-') || (argv[argc - 2][0] == '-' && argc - 3 == 0) || (figure->fill && (strcmp(argv[argc - 2], "--fill") || (strcmp(argv[argc - 2], "-f"))))) )
        figure->start_filename = argv[argc - 1];
}

/**
 * @brief Функция, которая парсит флаг --angle.
 * @param figure Указатель на фигуру.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль в случае удачи, код ошибки в ином случае.
 */
int angle(object_t* figure, int argc, char** argv) {
    int count_args = is_correct_count_args(argc, argv, "--angle");
    if (count_args != 0) return count_args;

    if (!is_number(optarg)) {
        fprintf(stderr, "Ошибка: вы ввели  не число для --angle\n");
        return -1;
    }

    int res = sscanf(optarg, "%d", &figure->angle);

    if (res != 1) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для --angle\n");
        return -1;
    }

    if (figure->angle != 90 && figure->angle != 180 && figure->angle != 270) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для, должны быть 90 или 180 или 270 --angle\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Данная функция парсит флаг --circle.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль в случае успеха, в ином случае код ошибки.
 */
int circle_get(object_t* figure, int argc, char** argv) {
    int result_coords = parce_coords(argc, argv, "--circle");
    if (result_coords != 0) return result_coords;

    int x, y;
    int res = sscanf(optarg, "%d.%d", &x, &y);

    if (is_correct_coords(x, y, res))
        return 1;

    figure->x_center = x; figure->y_center = y;
    return 0;

}

/**
 * @brief Данная функция парсит флаг --radius.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return В случае успеха ноль, в ином случае код ошибки.
 */
int radius(object_t* figure, int argc, char** argv) {
    int result_coords = parce_coords(argc, argv, "--radius");
    if (result_coords != 0) return result_coords;

    if (!is_number(optarg)) {
        fprintf(stderr, "Ошибка: вы ввели  не число для --radius\n");
        return -1;
    }

    int res = sscanf(optarg, "%d", &figure->radius);

    if (res != 1) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для --radius\n");
        return -1;
    }

    if (figure->radius <= 0) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для, должны быть больше нуля! --radius\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Данная функция парсит флаг --count.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return В случае успеха ноль, в ином случае код ошибки.
 */
int count(object_t* figure, int argc, char** argv) {
    int result_args = is_correct_count_args(argc, argv, "--count");
    if (result_args != 0) return result_args;


    if (!is_number(optarg)) {
        fprintf(stderr, "Ошибка: вы ввели  не число для --count\n");
        return -1;
    }

    int res = sscanf(optarg, "%d", &figure->count);

    if (res != 1) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для --count\n");
        return -1;
    }

    if (figure->count <=0 ) {
        fprintf(stderr, "Ошибка:  аргументы неправильные для --count\n");
        return -1;
    }

    return 0;
}

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
 * @brief Данная функция является парсером --rect.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
int parce_rectangle(int argc, char** argv, object_t* figure) {
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
                int result_l = left_up(figure, argc, argv);
                if (result_l != 0) return result_l;
                break;
            }
            case 'r': {
                int result_r = right_down(figure, argc, argv);
                if (result_r != 0) return result_r;
                break;
            }
            case 't': {
                int result_t = thickness(figure, argc, argv);
                if (result_t != 0) return result_t;
                break;
            }
            case 'c': {
                int result_c = color(figure, argc, argv);
                if (result_c != 0) return result_c;
                break;
            }
            case 'f':
                if (optind < argc && argv[optind][0] != '-' && !(isalpha(argv[optind][0]) && optind  + 1 == argc)) {
                    fprintf(stderr, "Ошибка: --fill не принимает аргументы!\n");
                    return -1;
                }
                figure->fill = true;
                break;
            case 'F': {
                int result_f_c = fill_color(figure, argc, argv);
                if (result_f_c != 0) return result_f_c;
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

/**
 * @brief Данная функция является парсером для --circle.
 * @param figure Указатель на объект.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль если все хорошо, в ином случае код ошибки.
 */
int parce_circle(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"center", required_argument, 0, 'c'},
        {"radius", required_argument, 0, 'r'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'C'},
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:c:r:C:fF:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': {
                int result_c = circle_get(figure, argc, argv);
                if (result_c != 0) return result_c;
                break;
            }
            case 'r': {
                int result_rad = radius(figure, argc, argv);
                if (result_rad != 0) return result_rad;
                break;
            }
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
            case 'f':
                if (optind < argc && argv[optind][0] != '-') {
                    fprintf(stderr, "Ошибка: --fill не принимает аргументы!\n");
                    return -1;
                }
                figure->fill = true;
                break;
            case 'F': {
                int result_f_c = fill_color(figure, argc, argv);
                if (result_f_c != 0) return result_f_c;
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

// Пока так.
void print_help() {
    printf("Course work for option 5.1, created by Ivan Aleinikov\n");
}

/**
 * @brief Данная функция проверяет, имеет ли файл расширение .bmp.
 * @param filename Имя файла.
 * @return Да или нет
 */
bool check_dt_bmp(const char* filename) {
    size_t len = strlen(filename);
    return !(len >= 4 && strcmp(filename + len - 4, ".bmp") == 0);
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

    if (argc == 1) {
        return 1;
    }
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
        help_print();
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
                if(!optarg){
                    fprintf(stderr, "Error: не было передано название файла!\n");
                    return 1;
                }
                figure->start_filename = optarg;
                res = 0;
                break;

            case help:
                print_help();
                help_print();
                return 69;

            case '?':
                fprintf(stderr, "Error: была переданна неизвестная коменда!\n");
                return 1;
            default:
                printf("fdfd");
        }
        return res;
    }



    return 0;
}

/**
 * @brief Проверят на корректность заполненость цветов для определённого режима.
 * @param figure Указатель на объект
 * @return Правда или ложь.
 */
bool checker_color(object_t* figure) {
    if (figure->color_r == -1 || figure->color_g == -1 || figure->color_b == -1) {
        fprintf(stderr, "Error: Вы не ввели цвет для фигуры!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненость поля thinckness.
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_thinckness(object_t* figure) {
    if (figure->thinckness == -1) {
        fprintf(stderr, "Error: Вы не ввели толщину линии!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненость полей координат левой точки.
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_left_up(object_t* figure) {
    if (figure->x_left_up == -1 || figure->y_left_up == -1) {
        fprintf(stderr, "Error: Вы не ввели координаты для левой верхней точки!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненость полей координат правой точки.
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_right_down(object_t* figure) {
    if (figure->x_right_down == -1 || figure->y_right_down == -1) {
        fprintf(stderr, "Error: Вы не ввели координаты для правой нижней точки!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Данная функция проверяет на корректность заполненость полей заливки и цвета заливки.
 * @param figure Указатель на объект
 * @return Правду либо ложь.
 */
bool checker_fill(object_t* figure) {
    if (figure->fill && (figure->color_fill_b == -1 || figure->color_fill_r == -1 || figure->color_fill_g == -1)) {
        fprintf(stderr, "Error: Вы выбрали режим заливки, но не выбрали цвет!\n");
        return 1;
    }

    if (!figure->fill && (figure->color_fill_b != -1 || figure->color_fill_r != -1 || figure->color_fill_g != -1)) {
        fprintf(stderr, "Error: Вы не выбрали режим заливки, но выбрали цвет!\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Данная функция проверяет корректность заполнености структуры для режима работы --rect.
 * @param figure Указатель на структуру.
 * @return Ноль или единица
 */
int check_rectangle(object_t* figure) {
    if (checker_left_up(figure)) return 1;

    if (checker_right_down(figure)) return 1;

    if (checker_thinckness(figure)) return 1;

    if (checker_color(figure)) return 1;

    if (checker_fill(figure)) return 1;

    return 0;
}

/**
 * @brief Данная функция проверяет корректность заполнености структуры для режима работы --circle.
 * @param figure Указатель на структуру.
 * @return Ноль или единица
 */
int check_circle(object_t* figure) {
    // Проверяем специфические параметры
    if (figure->x_center == -1 || figure->y_center == -1) {
        fprintf(stderr, "Error: Вы не выбрали центр окружности!\n");
        return 1;
    }

    if (figure->radius == -1 ) {
        fprintf(stderr, "Error: Вы не выбрали радиус окружности!\n");
        return 1;
    }

    if (checker_thinckness(figure)) return 1;

    if (checker_color(figure)) return 1;

    if (checker_fill(figure)) return 1;

    return 0;
}

/**
 * @brief Проверят корректность полей для ornament.
 * @param figure Указатель на объект.
 * @return Ноль в случае удачи в ином случае единица.
 */
int check_ornament(object_t* figure) {

    if (figure->color_r == -1 || figure->color_g == -1 || figure->color_b == -1) {
        fprintf(stderr, "Error: Вы не ввели цвет для фигуры!\n");
        return 1;
    }

    switch (figure->pattern) {
        case rectangle:
            if (checker_color(figure)) return 1;
            if (checker_thinckness(figure)) return 1;

            if (figure->count == -1) {
                fprintf(stderr, "Error: Вы не ввели количество!\n");
                return 1;
            }

            break;
        case circle: // У него только цвет
            break;
        case semicircle:
            if (checker_color(figure)) return 1;
            if (checker_thinckness(figure)) return 1;
            if (figure->count == -1) {
                fprintf(stderr, "Error: Вы не ввели количество!\n");
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Error: Невозможная ошибка в проверка орнамента.\n");
            return 1
        ;
    }

    return 0;
}

/**
 * @brief Проверят корректность полей для ornament.
 * @param figure Указатель на объект.
 * @return Ноль в случае удачи в ином случае единица.
 */
int check_rotate(object_t* figure) {
    if (checker_left_up(figure)) return 1;

    if (checker_right_down(figure)) return 1;

    if (figure->angle == -1 ) {
        fprintf(stderr, "Error: Вы не выбрали угол поворота!\n");
        return 1;
    }

    return 0;
}

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
        case info:   // Избыточно, так как уже проверяется корректность имени файла.
            break;
        default:
            fprintf(stderr, "Error: Невозможная ошибка!\n");
            return 1;
    }

    return res_check;
}

int main(int argc, char* argv[]){
    object_t* figure = malloc(sizeof(object_t));
    constuctor(figure);

    int res = base_parser(figure, argc, argv);
    if (res != 0) return 41;

    int result_chek = base_checker(figure);
    if (result_chek != 0) return 42;

    printf("mod %d\n", figure->mod);
    printf("pattern %d\n", figure->pattern);
    printf(" x_lu %d\n", figure->x_left_up);
    printf("y_lu %d\n", figure->y_left_up);
    printf("x_rd %d\n", figure->x_right_down);
    printf("y_rd %d\n", figure->y_right_down);
    printf("thincless %d\n", figure->thinckness);
    printf("color %d  %d  %d\n", figure->color_r, figure->color_g, figure->color_b);
    printf("fill %d\n", figure->fill);
    printf("color_fill %d  %d  %d\n", figure->color_fill_r, figure->color_fill_g, figure->color_fill_b);
    printf("count %d\n", figure->count);
    printf("angle  %d\n", figure->angle);
    printf("x_c, y_c %d  %d\n", figure->x_center, figure->y_center);
    printf("rad %d\n", figure->radius);


    puts(figure->start_filename);

    puts(figure->finish_filename);
    free(figure);

    return 0;
}


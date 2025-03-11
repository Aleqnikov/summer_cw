/**
* @file additional.c
 * @brief Содержит вспомогательные функции для парсера.
 */

#include "additional.h"

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
 * @brief Данная функция проверят, передал ли пользователь лишний аргумент, после флага, не требующего аргументы.
 *
 *  Функция проверят, что аргументов больше двух ( в противном случае доп аргумента попросту и нету)
 *  затем она проверят, что следующий аргумент не флаг, в случае неудачи возвращает ложь.
 *
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Булевое значение.
 */
bool no_arguments(int argc, char** argv, char* flag_name) {
    if (argc > 2 && argv[2][0] != '-') {
        fprintf(stderr, "Error:  %s не принимает аргументов!\n", flag_name);
        return 1;
    }
    return 0;
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

    if (optarg[0] == '-' && !isdigit(optarg[1])) {
        fprintf(stderr, "Error: вы не ввели аргументы для функции %s!\n", name);
        return 1;
    }

    if ( optind < argc && argv[optind][0] != '-' && !(isalpha(argv[optind][0]) && optind  + 1 == argc)) {
        fprintf(stderr, "Error: вы ввели слишком много аргументов для %s\n", name);
        return 1;
    }

    return 0;
}

/**
 * @brief Функция сравнения считанных аргументов, и ожидаемого количества.
 *
 * Данная функция проверяет, совпадает ли количество аргументов,
 * переданных в программу, с ожидаемым количеством. Если количество
 * считанных аргументов не соответствует ожидаемому, функция
 * выводит сообщение об ошибке в стандартный поток ошибок
 * и возвращает значение 1 (истина). В противном случае
 * возвращает 0 (ложь).
 *
 * @param read_count Общее количество аргументов, которые были прочитаны.
 * @param correct_count Ожидаемое количество аргументов.
 * @param name_mode Имя режима работы программы, используемое для
 * вывода в сообщение об ошибке.
 *
 * @return true (1), если количество аргументов не совпадает,
 * иначе false (0).
 */
bool comp_cnt_args(int read_count, int correct_count, char* name_mode) {
    if (read_count != correct_count) {
        fprintf(stderr, "Error: Некорректно переданы аргументы для  %s\n", name_mode);
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
bool check_color(int argc, char** argv, char* name) {
    int count_args = is_correct_count_args(argc, argv, name);
    if (count_args != 0) return count_args;

    if (is_correct_dots(optarg, 2)) {
        fprintf(stderr, "Error: Цвета переданы в некорректном формате! Должно быть - r.g.b\n");
        return 1;
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
bool check_colors(int count_read, int red, int green, int blue) {
    if (comp_cnt_args(count_read, three_read, ""))
        return 1;

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
bool parce_coords(int argc, char** argv, char* name) {
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
bool left_up(object_t* figure, int argc, char** argv) {
    int result_coords = parce_coords(argc, argv, "left_up");
    if (result_coords != 0)
        return result_coords;

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (comp_cnt_args(count_read, two_read, "--left_up"))
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
bool right_down(object_t* figure, int argc, char** argv) {
    int result_coords = parce_coords(argc, argv, "right_down");
    if (result_coords != 0)
        return result_coords;

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (comp_cnt_args(count_read, two_read, "--right_down"))
        return 1;

    figure->x_right_down = x; figure->y_right_down = y;
    return 0;
}

/**
 * @brief Данная функция явяется реализацией парсера для флага --thickness.
 * @param figure Указатель на объект данных.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль, если все в порядке, в ином случает 1.
 */
bool thickness(object_t* figure, int argc, char** argv) {
    int count_args = is_correct_count_args(argc, argv, "--thickness");
    if (count_args != 0) return count_args;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --thickness\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->thinckness);

    if (comp_cnt_args(count_read, one_read, "--thickness"))
        return 1;

    if (figure->thinckness <= 0) {
        fprintf(stderr, "Error: Неккоректный аргумент для --thickness, должно быть больше нуля!\n");
        return 1;
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
bool color(object_t* figure, int argc, char** argv) {
    int first_check = check_color(argc, argv, "--color");
    if (first_check != 0) return first_check;

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (check_colors(count_read, red, green, blue))
        return 1;

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
bool fill_color(object_t* figure, int argc, char** argv) {
    int first_check = check_color(argc, argv, "--fill_color");
    if (first_check != 0) return first_check;

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (check_colors(count_read, red, green, blue))
        return 1;

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
bool input_name(object_t* figure, int argc, char** argv) {
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
bool output_name(object_t* figure, int argc, char** argv) {
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
    if (figure->start_filename != NULL)
        return;

    bool has_enough_args = (argc - 2 > 0);
    bool previous_is_not_flag = (argv[argc - 2][0] == '-' && isdigit(argv[argc - 2][1])) || argv[argc - 2][0] != '-' ;
    bool only_fill_flag_present = (argv[argc - 2][0] == '-' && argc - 3 == 0);
    bool is_fill_flag = figure->fill &&
                        (strcmp(argv[argc - 2], "--fill") == 0 ||
                         strcmp(argv[argc - 2], "-f") == 0);

    if ((has_enough_args && previous_is_not_flag) ||
        only_fill_flag_present ||
        is_fill_flag) {
        figure->start_filename = argv[argc - 1];
        }
}

/**
 * @brief Функция, которая парсит флаг --angle.
 * @param figure Указатель на фигуру.
 * @param argc Количество аргументов.
 * @param argv Список аргументов.
 * @return Ноль в случае удачи, код ошибки в ином случае.
 */
bool angle(object_t* figure, int argc, char** argv) {
    if (is_correct_count_args(argc, argv, "--angle"))
        return 1;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели не число для --angle\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->angle);

    if (comp_cnt_args(count_read, one_read, "--angle"))
        return 1;

    if (figure->angle != 90 && figure->angle != 180 && figure->angle != 270) {
        fprintf(stderr, "Error: Введёно неправильно значение для --angle должны быть 90 или 180 или 270.\n");
        return 1;
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
bool circle_get(object_t* figure, int argc, char** argv) {
    if (parce_coords(argc, argv, "--circle"))
        return 1;

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (comp_cnt_args(count_read, two_read, "--center"))
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
bool radius(object_t* figure, int argc, char** argv) {
    if (parce_coords(argc, argv, "--radius"))
        return 1;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --radius\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->radius);

    if (comp_cnt_args(count_read, one_read, "--radius"))
        return 1;

    if (figure->radius <= 0) {
        fprintf(stderr, "Ошибка: Радиус должен быть больше нуля! --radius\n");
        return 1;
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
bool count(object_t* figure, int argc, char** argv) {
    if (is_correct_count_args(argc, argv, "--count"))
        return 1;

    if (!is_number(optarg)) {
        fprintf(stderr, "Error: аргумент для --count должен быть числом!\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->count);

    if (comp_cnt_args(count_read, one_read, "--count"))
        return 1;

    if (figure->count <=0 ) {
        fprintf(stderr, "Error:  аргументы неправильные для --count\n");
        return 1;
    }

    return 0;
}
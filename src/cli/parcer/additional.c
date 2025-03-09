#include "additional.h"

/**
 * @brief Данная функция проверяет корректность аргументов, проверяет что строку полностью состоит из чисел
 * @param str строка, которую нужно проверить
 * @return Если есть что то, кроме чисел, то возвращает ноль, иначе единицу.
 */
bool isnumber(const char *str) {
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
        fprintf(stderr, "Error: Неккоректные координаты! Их должно быть двое!\n");
        return 1;
    }


    if (x <= 0 || y <= 0) {
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
        fprintf(stderr, "Error: Координаты переданы в некоректном формате! Должно быть - x.y\n");
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

    if (!isnumber(optarg)) {
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
    int check_args = is_correct_count_args(argc, argv, "input_name");
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
    if (figure->start_filename == NULL && argc - 2 > 0 && argv[argc - 2][0] != '-')
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

    if (!isnumber(optarg)) {
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

    if (!isnumber(optarg)) {
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


    if (!isnumber(optarg)) {
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
#include "additional.h"

bool isnumber(const char *str) {
    if (!str || *str == '\0') return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-') return 0;
        str++;
    }
    return 1;
}

// Быть может в дальнейшем соединить с iscolor
bool iscoords(const char *str) {

    char *first_dot = strchr(str, '.');
    if (!first_dot)
        return 0;

    char *second_dot = strchr(first_dot + 1, '.');

    if (!str || *str == '\0')
        return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-' && *str != '.') return 0;
        str++;
    }

    return !(second_dot == NULL);
}

bool iscolor(const char *str) {
    char *first_dot = strchr(str, '.');
    if (!first_dot)
        return 0;

    char *second_dot = strchr(first_dot + 1, '.');


    char* theird_dot = (second_dot == NULL) ? second_dot : strchr(second_dot + 1, '.');

    if (!str || *str == '\0')
        return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-' && *str != '.') return 0;
        str++;
    }

    return !(theird_dot == NULL);
}

int parce_coords(char* optarg, int optind, int argc, char** argv, char* name) {
    if ( optind < argc && argv[optind][0] != '-') {
        fprintf(stderr, "Ошибка: вы ввели слишком много аргументов для %s\n", name);
        return -1;
    }

    // Вторая проверка, на количество точек, гарантирует, что передано правильно количество аргументов.
    if (iscoords(optarg)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате! Должно быть - x.y\n");
        return -1;
    }

    return 0;
}

int left_up(object_t* figure, int argc, char** argv) {
    // Проверка корректности ввода координат
    int result_coords = parce_coords(optarg, optind, argc, argv, "left_up");
    if (result_coords != 0)
        return result_coords;

    int res = sscanf(optarg, "%d.%d", &figure->x_left_up, &figure->y_left_up);

    if (res != 2) {
        fprintf(stderr, "Error: передано слишком мало аргументов флагу left_up\n");
        return 1;
    }

    return 0;
}

int right_down(object_t* figure, int argc, char** argv) {
    // Проверка корректности ввода координат
    int result_coords = parce_coords(optarg, optind, argc, argv, "right_down");
    if (result_coords != 0)
        return result_coords;

    int res = sscanf(optarg, "%d.%d", &figure->x_right_down, &figure->y_right_down);

    if (res != 2) {
        fprintf(stderr, "Error: передано слишком мало аргументов флагу right_down\n");
        return 1;
    }

    return 0;
}

int thickness(object_t* figure, int argc, char** argv) {
    if ( optind < argc && argv[optind][0] != '-') {
        fprintf(stderr, "Ошибка: вы ввели слишком много аргументов для --thickness\n");
        return -1;
    }

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

// Как то объединить с fill_color
int color(object_t* figure, int argc, char** argv) {
    if ( optind < argc && argv[optind][0] != '-') {
        fprintf(stderr, "Ошибка: вы ввели слишком много аргументов для --thickness\n");
        return -1;
    }

    // Вторая проверка, на количество точек, гарантирует, что передано правильно количество аргументов.
    if (iscolor(optarg)) {
        fprintf(stderr, "Error: Цвета переданы в некоректном формате! Должно быть - r.g.b\n");
        return -1;
    }

    int res = sscanf(optarg, "%d.%d.%d", &figure->color_r, &figure->color_g, &figure->color_b);

    if (res != 3) {
        fprintf(stderr, "Error: передано слишком мало аргументов флагу color\n");
        return 1;
    }

    if (figure->color_r < 0 || figure->color_r > 255 || figure->color_g < 0 || figure->color_g > 255 || figure->color_b < 0 || figure->color_b > 255) {
        fprintf(stderr, "Error: цвета переданы в неккоректном формате! Каждый должен быть в диапазоне [0, 255]");
        return 1;
    }

    return 0;

}

int fill_color(object_t* figure, int argc, char** argv) {
    if ( optind < argc && argv[optind][0] != '-') {
        fprintf(stderr, "Ошибка: вы ввели слишком много аргументов для --thickness\n");
        return -1;
    }

    // Вторая проверка, на количество точек, гарантирует, что передано правильно количество аргументов.
    if (iscolor(optarg)) {
        fprintf(stderr, "Error: Цвета переданы в некоректном формате! Должно быть - r.g.b\n");
        return -1;
    }

    int res = sscanf(optarg, "%d.%d.%d", &figure->color_fill_r, &figure->color_fill_g, &figure->color_fill_b);

    if (res != 3) {
        fprintf(stderr, "Error: передано слишком мало аргументов флагу --fill_color\n");
        return 1;
    }

    if (figure->color_fill_r < 0 || figure->color_fill_r > 255 || figure->color_fill_g < 0 || figure->color_fill_g > 255 || figure->color_fill_b < 0 || figure->color_fill_b > 255) {
        fprintf(stderr, "Error: цвета переданы в неккоректном формате! Каждый должен быть в диапазоне [0, 255]");
        return 1;
    }

    return 0;
}

int input_name(object_t* figure, int argc, char** argv) {
    if (!optarg) {
        fprintf(stderr, "Error: Не был передан аргумент для --input!\n");
        return 1;
    }

    figure->start_filename = optarg;
    return 0;
}

int output_name(object_t* figure, int argc, char** argv) {
    if (!optarg) {
        fprintf(stderr, "Error: Не был передан аргумент для --output!\n");
        return 1;
    }

    figure->finish_filename = optarg;
    return 0;
}

void get_filename(object_t* figure, int argc, char** argv) {
    if (figure->start_filename == NULL)
        figure->start_filename = argv[argc - 1];

}

int angle(object_t* figure, int argc, char** argv) {
    if ( optind < argc && argv[optind][0] != '-') {
        fprintf(stderr, "Ошибка: вы ввели слишком много аргументов для --angle\n");
        return -1;
    }

    if (!isnumber(optarg)) {
        fprintf(stderr, "Ошибка: вы ввели  не число для --angle\n");
        return -1;
    }

    int res = sscanf(optarg, "%d", &figure->angle);

    if (res != 1) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для --angle\n");
        return -1;
    }

    if (figure->thinckness != 90 && figure->thinckness != 180 && figure->thinckness != 270) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для, должны быть 90 или 180 или 270 --angle\n");
        return -1;
    }

    return 0;
}

int circle_get(object_t* figure, int argc, char** argv) {
    // Проверка корректности ввода координат
    int result_coords = parce_coords(optarg, optind, argc, argv, "circle");
    if (result_coords != 0)
        return result_coords;

    int res = sscanf(optarg, "%d.%d", &figure->x_center, &figure->y_center);

    if (res != 2) {
        fprintf(stderr, "Error: передано слишком мало аргументов флагу circle\n");
        return 1;
    }

    return 0;
}

int radius(object_t* figure, int argc, char** argv) {
    if ( optind < argc && argv[optind][0] != '-') {
        fprintf(stderr, "Ошибка: вы ввели слишком много аргументов для --radius\n");
        return -1;
    }

    if (!isnumber(optarg)) {
        fprintf(stderr, "Ошибка: вы ввели  не число для --radius\n");
        return -1;
    }

    int res = sscanf(optarg, "%d", &figure->radius);

    if (res != 1) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для --radius\n");
        return -1;
    }

    if (figure->thinckness <= 0) {
        fprintf(stderr, "Ошибка: вы аргументы неправильные для, должны быть больше нуля! --radius\n");
        return -1;
    }

    return 0;
}


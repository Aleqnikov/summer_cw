#include "parcer_assets.h"

bool parce_pattern(char* optarg, object_t* figure) {
    if (!strcmp(optarg, "rectangle")) figure->pattern = rectangle;
    if (!strcmp(optarg, "semicircles")) figure->pattern = semicircle;
    if (!strcmp(optarg, "circle")) figure->pattern = circle;

    if (figure->pattern == none) {
        fprintf(stderr, "Error: Вы выбрали несушествующий режим орнамента!\n");
        return 1;
    }

    return 0;
}

bool parce_left_up(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (count_read != 2) {
        fprintf(stderr, "Error: координаты переданы в неправильном формате!\n");
        return 1;
    }

    figure->x_left_up = x; figure->y_left_up = y;

    return 0;
}

bool parce_right_down(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (count_read != 2) {
        fprintf(stderr, "Error: координаты переданы в неправильном формате!\n");
        return 1;
    }

    figure->x_right_down = x; figure->y_right_down = y;

    return 0;
}

bool parce_angle(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели не число для --angle\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->angle);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --angle\n");
        return 1;
    }

    if (figure->angle != 90 && figure->angle != 180 && figure->angle != 270) {
        fprintf(stderr, "Error: Введёно неправильно значение для --angle должны быть 90 или 180 или 270.\n");
        return 1;
    }

    return 0;
}

bool parce_count(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: аргумент для --count должен быть числом!\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->count);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --count\n");
        return 1;
    }

    if (figure->count <=0 ) {
        fprintf(stderr, "Error: Введены отрицательные аргументы для --count\n");
        return 1;
    }

    return 0;
}

bool parce_thickness(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --thickness\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->thinckness);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --thinckness\n");
        return 1;
    }

    if (figure->thinckness <= 0) {
        fprintf(stderr, "Error: Неккоректный аргумент для --thickness, должно быть больше нуля!\n");
        return 1;
    }

    return 0;
}

bool parce_color(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 2)) {
        fprintf(stderr, "Error: Цвет передан в некоректном формате!\n");
        return 1;
    }

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (count_read != 3) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --color\n");
        return 1;
    }

    if (check_colors(red, green, blue))
        return 1;

    figure->color_r = red; figure->color_g = green; figure->color_b = blue;
    return 0;
}

bool parce_fill_color(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 2)) {
        fprintf(stderr, "Error: Цвет передан в некоректном формате!\n");
        return 1;
    }

    int red, green, blue;
    int count_read = sscanf(optarg, "%d.%d.%d", &red, &green, &blue);

    if (count_read != 3) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --fill_color\n");
        return 1;
    }

    if (check_colors(red, green, blue))
        return 1;

    figure->color_fill_r = red; figure->color_fill_g = green; figure->color_fill_b = blue;
    return 0;
}

bool parce_center(const char* optarg, object_t* figure) {
    if (is_correct_dots(optarg, 1)) {
        fprintf(stderr, "Error: Координаты переданы в некоректном формате!\n");
        return 1;
    }

    int x, y;
    int count_read = sscanf(optarg, "%d.%d", &x, &y);

    if (count_read != 2) {
        fprintf(stderr, "Error: координаты переданы в неправильном формате!\n");
        return 1;
    }

    figure->x_center = x; figure->y_center = y;

    return 0;
}

bool parce_radius(const char* optarg, object_t* figure) {
    if (!is_number(optarg)) {
        fprintf(stderr, "Error: вы ввели  не число для --radius\n");
        return 1;
    }

    int count_read = sscanf(optarg, "%d", &figure->radius);

    if (count_read != 1) {
        fprintf(stderr, "Error: Вы ввели неккоректное значение для --radius\n");
        return 1;
    }

    if (figure->radius <= 0) {
        fprintf(stderr, "Error: Неккоректный аргумент для --radius, должно быть больше нуля!\n");
        return 1;
    }

    return 0;

}

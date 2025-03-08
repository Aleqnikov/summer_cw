#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum Pattern {rectangle, circle, semicircle} pattern_t;

typedef enum Mode {rect, ornament, rotate, circ} mode;


/*
* Структура, которая хранит информацию о действиях, которые нужно сделать!
*
*/
typedef struct Object {
    mode mod;

    char* start_filename;
    char* finish_filename;

    int x_left_up, y_left_up;
    int x_right_down, y_right_down;

    int thinckness;

    int color_r, color_g, color_b;

    bool fill;

    int color_fill_r, color_fill_g, color_fill_b;

    pattern_t pattern;

    int count;

    int angle;

    int x_center, y_center;
    int radius;
} object_t ;

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

int parce_rectangle(int argc, char** argv, object_t* figure){

    static struct option long_options[] = {
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'c'},
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0,0,0,0},
    };

    int opt, option_index = 0;

    while((opt = getopt_long(argc, argv, "l:r:t:c:fF:i:o:", long_options, &option_index)) != -1){
        switch (opt) {
            case 'l':
                int result_l = left_up(figure, argc, argv);

                if (result_l != 0) return result_l;
                break;

            case 'r':
                int result_r = right_down(figure, argc, argv);

                if (result_r != 0) return result_r;
                break;
            case 't':
                int result_t = thickness(figure, argc, argv);

                if (result_t != 0) return result_t;
                break;

            case 'c':
                int result_c = color(figure, argc, argv);

                if (result_c != 0) return result_c;
                break;

            case 'f':
                if ( optind < argc && argv[optind][0] != '-') {
                    fprintf(stderr, "Ошибка: --fill не принимает аргументы!\n");
                    return -1;
                }
                figure->fill = true;
                break;

            case 'F':
                int result_f_c = fill_color(figure, argc, argv);

                if (result_f_c != 0) return result_f_c;
                break;
            case 'i':
                int res_i = input_name(figure, argc, argv);
                if (res_i != 0) return res_i;
                break;
            case 'o':
                int res_o = output_name(figure, argc, argv);
                if (res_o != 0) return res_o;
                break;

            case '?':
                fprintf(stderr, "Ошибка: лишние аргументы");
                break;
        }
    }

    get_filename(figure, argc, argv);

    return 0;
}

int parce_rotate(int argc, char** argv, object_t* figure) {
    static struct option long_options[] = {
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"angle", required_argument, 0, 'a'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0,0,0,0},
    };

    int opt, option_index = 0;

    while((opt = getopt_long(argc, argv, "l:r:a:i:0:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'l':
                int result_l = left_up(figure, argc, argv);

                if (result_l != 0) return result_l;
                break;

            case 'r':
                int result_r = right_down(figure, argc, argv);

                if (result_r != 0) return result_r;
                break;

            case 'a':
                int result_a = angle(figure, argc, argv);

                if (result_a != 0) return result_a;
                break;

            case 'i':
                int res_i = input_name(figure, argc, argv);
                if (res_i != 0) return res_i;
                break;
            case 'o':
                int res_o = output_name(figure, argc, argv);
                if (res_o != 0) return res_o;
                break;

            case '?':
                fprintf(stderr, "Ошибка: лишние аргументы");
                break;

        }
    }

    get_filename(figure, argc, argv);
}

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
        {0,0,0,0},
    };

    int opt, option_index = 0;

    while((opt = getopt_long(argc, argv, "t:c:fF:i:o:", long_options, &option_index)) != -1){
        switch (opt) {
            case 'c':
                int result_c = circle_get(figure, argc, argv);

                if (result_c != 0) return result_c;
                break;

            case 'r':
                int result_rad = radius(figure, argc, argv);

                if (result_rad != 0) return result_rad;
                break;
            case 't':
                int result_t = thickness(figure, argc, argv);

                if (result_t != 0) return result_t;
                break;

            case 'C':
                int result_C = color(figure, argc, argv);

                if (result_C != 0) return result_C;
                break;

            case 'f':
                if ( optind < argc && argv[optind][0] != '-') {
                    fprintf(stderr, "Ошибка: --fill не принимает аргументы!\n");
                    return -1;
                }
                figure->fill = true;
                break;

            case 'F':
                int result_f_c = fill_color(figure, argc, argv);

                if (result_f_c != 0) return result_f_c;
                break;
            case 'i':
                int res_i = input_name(figure, argc, argv);
                if (res_i != 0) return res_i;
                break;
            case 'o':
                int res_o = output_name(figure, argc, argv);
                if (res_o != 0) return res_o;
                break;

            case '?':
                fprintf(stderr, "Ошибка: лишние аргументы");
                break;
        }
    }

    get_filename(figure, argc, argv);

    return 0;
}

void print_help() {
    printf("Course work for option 5.1, created by Ivan Aleinikov\n");
}


int base_parser(int argc, char** argv){
    static struct option long_options[] = {
        {"rect", no_argument, 0, 0},
        {"ornament", no_argument, 0, 0},
        {"rotate", no_argument, 0, 0},
        {"circle", no_argument, 0,0},
        {"info", required_argument, 0, 0},

        {"help", no_argument, 0, 'h'},
        {0,0,0,0},
    };

    int opt, option_index = 0;

    opt = getopt_long(argc, argv, "h", long_options, &option_index);

    object_t* figure = malloc(sizeof(object_t));


    if (opt == 'h'){
        print_help();
        return 1;

    }

    if (opt == 0) {
        switch (option_index) {
            case 0:
                figure->mod = rect;
                int res = parce_rectangle(argc, argv, figure);
                break;
            case 1:
                printf("option %s", long_options[option_index].name);
                break;
            case 2:
                figure->mod = rotate;
                int res_r = parce_rotate(argc, argv, figure);
                break;
            case 3:
                figure->mod = circ;
                int res_c = parce_circle(argc, argv, figure);
                break;
            case 4:
                printf("option %s", long_options[option_index].name);
                break;
            case 5:
                printf("option %s", long_options[option_index].name);
                break;
        }
    }

    return 1;
}

int main(int argc, char* argv[]){

    int res = base_parser(argc, argv);

    return 0;
}


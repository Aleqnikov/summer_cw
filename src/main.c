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

bool iscoords(const char *str) {



    char *first_dot = strchr(str, '.');
    if (!first_dot) {
        return 0;
    }

    char *second_dot = strchr(first_dot + 1, '.');

    if (!str || *str == '\0') return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-' && *str != '.') return 0;
        str++;
    }

    return !(second_dot == NULL);
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

int parce_rectangle(int argc, char** argv, object_t* figure){

    static struct option long_options[] = {
        {"left_up", required_argument, 0, 'l'},
        {"right_down", required_argument, 0, 'r'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'c'},
        {"fill", required_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'F'},
        {0,0,0,0},
    };

    int opt, option_index = 0;

    while((opt = getopt_long(argc, argv, "l:r:t:c:fF:", long_options, &option_index)) != -1){
        switch (opt){
        case 'l':
            int result_l = left_up(figure, argc, argv);

            if (result_l != 0) return result_l;
            break;

        case 'r':
            int result_r = right_down(figure, argc, argv);

            if (result_r != 0) return result_r;
            break;
        case 't':


            break;
        case 'c':
            printf("c\n");
            break;
        case 'f':
            printf("f\n");
            break;
            
        case 'F':
            printf("F\n");
            break;

        case '?':
            printf("Некорректная опция или аргумент.\n");
            break;

        default:
            printf("ИДИОТ, ОТЧИСЛЕН!!!!\n");
            break;
        }
    }

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
                printf("option %s", long_options[option_index].name);
                break;
            case 3:
                printf("option %s", long_options[option_index].name);
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


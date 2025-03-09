#include "parcer.h"


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
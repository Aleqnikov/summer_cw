#include "circle.h"



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
#include "help_canvas_fns.h"

/**
 * @brief Данная функция проверяет, выходит ли за пределы изображения координаты.
 */
bool check_coord(int y, int x, int height, int weight){
    if((x >= 0 && x < weight) && (y >= 0 && y < height))
        return 1;
    return 0;
}


Rgb** copy_array(Rgb** data, int height, int width){
    Rgb** new_array = NULL;
    new_array = (Rgb** )(malloc(sizeof(Rgb* ) * (height)));

    if (new_array == NULL)
        return NULL;

    for (int i = 0; i < (height); i++) {
        (new_array)[i] = (Rgb *)malloc(width * sizeof(Rgb));
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            new_array[i][j] = data[i][j];
        }
    }

    return new_array;
}

int custom_cos(int angle) {
    switch (angle) {
        case 90:
            return 0;
        case 180:
            return -1;
        case 270:
            return 0;
    }
}

int custom_sin(int angle) {
    switch (angle) {
        case 90:
            return 1;
        case 180:
            return 0;
        case 270:
            return -1;
    }
}
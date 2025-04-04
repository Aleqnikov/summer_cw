#include "canvas_circle.h"


// Function implementing Bresenham's circle algorithm for the entire first quarter of the circle
int circ_bre(int rad, point_t* circle){
    int x = 0;
    int y = rad;
    int d = 3 - 2 * rad;  // Initial decision parameter

    int index = 0;

    // Generate points for the first quarter (x >= 0, y >= 0, x² + y² ≈ r²)
    while (x <= y)
    {
        // Store the point in the first octant (x, y)
        circle[index].x = x;
        circle[index].y = y;
        index++;

        // Store the symmetric point in the second octant (y, x) — тоже в первой четверти!
        if (x != y)  // Чтобы не дублировать точку при x == y (на диагонали)
        {
            circle[index].x = y;
            circle[index].y = x;
            index++;
        }

        // Update decision parameter and coordinates
        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    return index;  // Return the number of points stored
}

int get_y_limit_up(point_t* circle, int len, int x_val, int x_c) {
    int y = INT_MIN;
    for (int i = 0; i < len; i++) {
        if ((circle[i]).x + x_c == x_val) {
            y = y >(circle[i]).y ? y : (circle[i]).y;
        }
    }

    if (y == INT_MIN)
        return 0;

    return y;
}

int get_y_limit_down(point_t* circle, int len, int x_val, int x_c) {
    int y = INT_MAX;
    for (int i = 0; i < len; i++) {

        if ((circle[i]).x + x_c == x_val) {
            y = y < (circle[i]).y ? y : (circle[i]).y;
        }
    }

    if (y == INT_MAX)
        return 0;
    return y;
}


int draw_thicnless(Rgb*** data, point_t* circle_m, point_t* circle_b, BitmapInfoHeader bmih, int len_min, int len_big, Rgb color, int x_c, int y_c, int radius, int thickness) {
    int x_min = x_c; int x_max = radius + x_c + thickness / 2;
    int y_min = y_c; int y_max = radius + y_c + thickness / 2;


    for (int j = x_min; j <= x_max; j++) {

        int y_up_lim = get_y_limit_up(circle_b, len_big, j, x_c);
        int y_down_lim = get_y_limit_down(circle_m, len_min, j, x_c);

        for (int i = y_down_lim; i <= y_up_lim; i++) {
            if (check_coord(y_min + i, j, bmih.height, bmih.width))
                (*data)[y_min + i][j] = color;
            if (check_coord(y_min - i, j, bmih.height, bmih.width))
                (*data)[y_min - i][j] = color;
            if (check_coord(y_min + i, 2*x_min - j, bmih.height, bmih.width))
                (*data)[y_min + i][2*x_min - j] = color;
            if (check_coord(y_min - i, 2* x_min - j, bmih.height, bmih.width))
                (*data)[y_min - i][2* x_min - j] = color;

        }


    }

    return 1;
}


int draw_fill_circle(Rgb*** data, point_t* circle_m, BitmapInfoHeader bmih, int rad, int len_min, Rgb color, int x_c, int y_c) {
    int x_min = x_c; int x_max = rad + x_c ;
    int y_min = y_c; int y_max = rad + y_c;


    for (int j = x_min; j <= x_max; j++) {

        int y_up_lim = get_y_limit_up(circle_m, len_min, j, x_c);

        for (int i = 0; i <= y_up_lim; i++) {
            if (check_coord(y_min + i, j, bmih.height, bmih.width))
                (*data)[y_min + i][j] = color;
            if (check_coord(y_min - i, j, bmih.height, bmih.width))
                (*data)[y_min - i][j] = color;
            if (check_coord(y_min + i, 2*x_min - j, bmih.height, bmih.width))
                (*data)[y_min + i][2*x_min - j] = color;
            if (check_coord(y_min - i, 2* x_min - j, bmih.height, bmih.width))
                (*data)[y_min - i][2* x_min - j] = color;
        }
    }

    return 1;
}

int draw_circle(Rgb*** data, BitmapInfoHeader bmih, Rgb color, Rgb color_fill, int thickness, int radius, int fill, int x_c, int y_c) {
    thickness = (thickness  % 2 == 0) ? thickness + 1 : thickness;

    point_t* min_circle = malloc(sizeof(point_t) * (radius - thickness / 2 )*8);
    point_t* big_circle = malloc(sizeof(point_t) * (radius + thickness / 2 )*8);

    int len_min = circ_bre(radius - thickness / 2, min_circle);
    int len_big = circ_bre(radius + thickness / 2, big_circle) ;


    if (fill) {
        draw_fill_circle(data,  min_circle, bmih, radius - thickness/ 2, len_min, color_fill, x_c, y_c);
    }

    draw_thicnless(data, min_circle, big_circle, bmih, len_min, len_big, color, x_c, y_c, radius, thickness);

    free(min_circle);
    free(big_circle);

    return 0;

}
#include "canvas_ornament.h"


int rectangle_ornament(Rgb*** data, BitmapInfoHeader bmih, int count, int thickness, Rgb color) {
    thickness = (thickness  % 2 == 0) ? thickness + 1 : thickness;

    int diff = thickness/2;
    for (int i = 0; i < count; i++) {
        draw_rectangle(data, bmih, false, color, color, thickness, diff, bmih.width - diff, bmih.height - diff, diff);
        diff += 2*thickness;
    }
    return 1;
}

int circle_ornament(Rgb*** data, BitmapInfoHeader bmih, Rgb color) {
    int x_c = (bmih.width + 1) / 2;
    int y_c = (bmih.height + 1) / 2;
    int min_radius = x_c < y_c ? x_c : y_c;
    int max_radius =  x_c + y_c;

    int thickness = 2*(max_radius - min_radius);

    draw_circle(data, bmih, color, color, thickness, max_radius, false, x_c, y_c);
}

int semi_circle_ornament(Rgb*** data, BitmapInfoHeader bmih, int count, int thickness, Rgb color) {
    int width_radius = bmih.width % (count * 2) == 0?  bmih.width / (count * 2):  bmih.width / (count * 2) + 1;
    int height_radius = bmih.height % (count * 2) == 0?  bmih.height / (count * 2):  bmih.height / (count * 2) + 1;

    int diff_width = width_radius;
    int diff_height = bmih.height - height_radius - 1;

    for (int i = 0; i < count; i++) {
        draw_circle(data, bmih, color, color, thickness, width_radius, false, diff_width, 0);
        draw_circle(data, bmih, color, color, thickness, width_radius, false, diff_width, bmih.height);

        draw_circle(data, bmih, color, color, thickness, height_radius, false, 0, diff_height);
        draw_circle(data, bmih, color, color, thickness, height_radius, false, bmih.width, diff_height);

        diff_width += 2*width_radius;
        diff_height -= 2*height_radius;
    }
    return 0;
}

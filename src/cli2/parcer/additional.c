/**
* @file additional.c
 * @brief В данном файле находятся вспомогательные функции для функций парсера.
 */

#include "additional.h"

/**
 * @brief Данная функция проверяет корректность аргументов, проверяет что строку полностью состоит из чисел
 *
 * Функция, которая проверят, что переданная строка является числом.
 * Используется в других функция при проверке на корректность аргументов.
 *
 * @param str строка, которую нужно проверить
 * @return Если число, то возвращает единицу.
 */
bool is_number(const char *str) {
    if (!str || *str == '\0') return 0;

    while (*str) {
        if (!isdigit(*str) && *str != '-') return 0;
        str++;
    }
    return 1;
}

/**
 * @brief Данная функция проверяет корректность аргументов парсера, которые заданы через точку (x.y).
 *
 * Функция проверят, что количество точек в данной строке соответствует нужному количеству.
 * В контексте данной программы она проверят, что строка соответствует формату <x.y>. То есть используется при
 * проверке корректности координат, и цветов. В основном используется как вспомогательная функция.
 *
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
 * @brief Вспомогательная функция, помогает определить корректность цвета, диапазон, и количество считанного.
 *
 *  Данная функция проверяет корректность переданных значений цветов.
 *  Используется как вспомогательная функция.
 *
 * @param count Количество прочитанных аргументов.
 * @param red Число красного цвета.
 * @param green Число зелёного цвета.
 * @param blue Число синего цвета.
 * @return Булевое значение.
 */
bool check_colors( int red, int green, int blue) {
    if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
        fprintf(stderr, "Error: цвета переданы в неккоректном формате! Каждый должен быть в диапазоне [0, 255]");
        return 1;
    }

    return 0;
}


bool set_mode(const char* mode, object_t* figure){
    if(figure->mod != None)
        return 1;
    if( strcmp(mode, "rect") == 0)
        figure->mod = rect;

    if( strcmp(mode, "ornament") == 0)
        figure->mod = ornament;


    if( strcmp(mode, "circle") == 0)
        figure->mod = circ;

    if( strcmp(mode, "rotate") == 0)
        figure->mod = rotate;


    if( strcmp(mode, "info") == 0)
        figure->mod = info;

    return 0;

}

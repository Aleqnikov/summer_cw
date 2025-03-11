#!/bin/bash

test_cases=(
  "--rect --left_up 0.0 --right_down 10.10 --thickness 1 --color 255.0.0 --fill --input image.bmp # Прямоугольник нарисован с заливкой"
  "--rect --left_up 0.0 --right_down 10.10 --thickness 1 --color 255.0.0 --input image.bmp # Прямоугольник нарисован без заливки"
  "--rect --left_up 0.0 --right_down 10.10 --thickness 1 --color 255.0.0 --fill --fill_color 0.255.0 --input image.bmp # Прямоугольник нарисован с зелёной заливкой"
  "--rect --left_up 0.0 --right_down 10.10 --color 255.0.0 --input image.bmp # Ошибка: не указана толщина линии"
  "--rect --left_up 0.0 --right_down 10.10 --color 255.0.0 --thickness 1 --fill_color 255.255.255 --input image.bmp # Ошибка: заливка не указана"
  "--rect --left_up 0.0 --right_down 10.10 --thickness -1 --color 255.0.0 --input image.bmp # Ошибка: недопустимая толщина"
  "--rect --left_up 0.0 --right_down 10.10 --color 500.0.0 --input image.bmp # Ошибка: неверный формат цвета"
  "--ornament --pattern rectangle --color 255.0.0 --thickness 1 --count 5 --input image.bmp # Нарисован орнамент с 5 прямоугольниками"
  "--ornament --pattern semicircles --color 255.0.0 --thickness 1 --count 2 --input image.bmp # Нарисован орнамент с 2 полукругами"
  "--rotate --left_up 0.0 --right_down 10.10 --angle 90 --input image.bmp # Изображение повернуто на 90 градусов"
  "--rotate --left_up 0.0 --right_down 10.10 --angle 180 --input image.bmp # Изображение повернуто на 180 градусов"
  "--circle --center 0.0 --radius 10 --thickness 1 --color 255.0.0 --input image.bmp # Нарисована окружность"
  "--circle --center 0.0 --radius 10 --thickness 1 --color 255.0.0 --fill --input image.bmp # Нарисована окружность с заливкой"
  "--circle --center 0.0 --radius 10 --thickness 1 --color 255.0.0 --fill --fill_color 0.255.0 --input image.bmp # Нарисована окружность с зелёной заливкой"
  "--circle --center 0.0 --radius -10 --thickness 1 --color 255.0.0 --input image.bmp # Ошибка: недопустимый радиус"
  "--circle --center 0.0 --radius 10 --thickness -1 --color 255.0.0 --input image.bmp # Ошибка: недопустимая толщина"
  "--help --input image.bmp # Отображение справки"
  "--info --input image.bmp # Отображение информации о изображении"
  "--info --input image.bmp --output out.bmp # Ошибка: лишний аргумент для --info"
  "--input image.bmp --output out.bmp # Ошибка: не указан основной флаг"
  "--circle --center 0.0 --radius 10 --thickness 1 --color 255.0.0 --fill --fill_color 255.255.255 --input image.bmp --output out.bmp # Нарисована белая окружность с заливкой и сохранена в out.bmp"
  "--rotate --left_up 0.0 --right_down 10.10 --angle 90 --input image.bmp --output out.png # Изображение повернуто и сохранено в out.png"
  "--ornament --pattern circle --color 255.0.0 --thickness 1 --count 3 --input image.bmp # Нарисован орнамент из 3 кругов"
)

for test_case in "${test_cases[@]}"
do
    echo "Running: ./a.out $test_case"
    eval "./a.out $test_case"
    echo ""
done

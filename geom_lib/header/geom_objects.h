#ifndef OBJECTS_H /* Страж подключения Objects.h */
#define OBJECTS_H


#include <stddef.h> // Для size_t


/* Структура, описывающая точку плоскости через 2 её координаты в д.с.к. */
typedef struct
{
    double x;
    double y;
} point_t;


/* Структура, описывающая отрезок плоскости через его начало и конец */
typedef struct
{
    point_t p1;
    point_t p2;
} segment_t;


/*
    Структура, описывающая многоугольник на плоскости.
    Хранит количество вершин и ссылку на первый элемент массива вершин.
*/
typedef struct
{
    size_t size; // Количество вершин многоугольника
    point_t * vertices; // Указатель на массив вершин
} polygon_t;


#endif /* OBJECTS_H */

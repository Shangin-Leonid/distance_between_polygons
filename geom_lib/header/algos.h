#ifndef ALGOS_H
#define ALGOS_H


#include "geom_objects.h"


/* Коды  проверки валидности объекта */
typedef enum{
    gv_VALID = 0,
    gv_IS_POINT = 1,
    gv_IS_SEGMENT = 2,
    gv_HAS_SELF_INTERSECTION = 3,
    gv_DEGENERATE = 4,
    gv_NOT_POLYGON = 5,
    gv_UNIDENTIFIED_ERROR = 6
} geom_validity_t;


/*
    Функция получает на вход две точки.
    Возвращает расстояние между ними.
*/
extern double Calc_dist_bet_points(point_t p1, point_t p2);


/*
    Функция получает на вход точку и отрезок плоскости.
    Возвращает  1, если точка принадлежит отрезку
                0, иначе
*/
extern int Does_point_belong_to_segment(point_t p, segment_t seg);


/*
    Функция получает на вход два отрезка.
    Возвращает  1, если отрезки пересекаются
                0, иначе.
*/
extern int Do_segments_intersect(segment_t seg_1, segment_t seg_2);


/*
    Функция получает на вход многоугольник на плоскости.
    Ошибками считаются многоугольники с числом сторон < 3 или многоугольники с самопересечениями.
    Возвращает код проверки его валидности.
*/
extern geom_validity_t Is_polygon_valid(polygon_t polygon);


/*
    Функция получает на вход точку и отрезок плоскости.
    Возвращает расстояние между ними.
*/
extern double Calc_dist_bet_point_and_segment(point_t p, segment_t seg);


/*
    Функция получает на вход два отрезка плоскости.
    Возвращает расстояние между ними.
*/
extern double Calc_dist_bet_segments(segment_t seg_1, segment_t seg_2);


/*
    Функция получает на вход два многоугольника (необязательно выпуклых и необязательно валидных)
    и ссылку на переменную кода ошибки.
    Ошибками считаются многоугольники с числом сторон < 3 или многоугольники с самопересечениями.
    Возвращает расстояние между входными многоугольниками.
*/
extern double Calc_dist_bet_polygons(polygon_t polygon_1, polygon_t polygon_2, geom_validity_t * err);


#endif /* ALGOS_H */

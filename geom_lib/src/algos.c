#include <stdlib.h>
#include <math.h>
#include <stdio.h>
// #define NDEBUG //REMOVE: отключает assert'ы
#include <assert.h>
#include "../header/geom_objects.h"
#include "../header/algos.h"


// Точность сравнения double-чисел в программе
#define EPS 1e-9


/* Макрос скалярного произведения двух направленых отрезков на плоскости */
#define SCALAR_PRODUCT(p1, p2, q1, q2) ((p2.x - p1.x)*(q2.x - q1.x) + (p2.y - p1.y)*(q2.y - q1.y))


/*
    Функция получает на вход две точки.
    Возвращает квадрат расстояния между ними.
*/
static inline double Calc_squared_dist_bet_points(point_t p1, point_t p2);



extern double Calc_dist_bet_points(point_t p1, point_t p2)
{ return hypot((p1.x - p2.x), (p1.y - p2.y)); }



extern int Does_point_belong_to_segment(point_t p, segment_t seg)
{
    /* A принадлежит отрезку [P1 P2] <=> \rho(A, P1) + \rho(A, P2) == \rho(P1, P2) */
    double p_p1 = Calc_dist_bet_points(p, seg.p1),
           p_p2 = Calc_dist_bet_points(p, seg.p2),
           p1_p2 = Calc_dist_bet_points(seg.p1, seg.p2);
    if(p_p1 + p_p2 - p1_p2 < EPS*p1_p2) // fabs ???
        return 1;
    return 0;
}



extern int Do_segments_intersect(segment_t seg1, segment_t seg2)
{
    /* Проверим такой случай пересечения: конец одного отрезка принадлежит другому отрезку (например "буква Т")*/
    if(Does_point_belong_to_segment(seg1.p1, seg2) ||
       Does_point_belong_to_segment(seg1.p2, seg2) ||
       Does_point_belong_to_segment(seg2.p1, seg1) ||
       Does_point_belong_to_segment(seg2.p2, seg1))
        return 1;

    /* Определители для решения методом Крамера системы пересечения прямых, содержащих отрезки */
    double delta,
           delta_x,
           delta_y;
    /*  Так выглядит расширенная матрица для системы пересечения прямых, содержащих отрезки:
        (seg1.p1.y - seg1.p2.y)   (seg1.p2.x - seg1.p1.x)  |  (seg1.p1.y*seg1.p2.x - seg1.p1.x*seg1.p2.y)
        (seg2.p1.y - seg2.p2.y)   (seg2.p2.x - seg2.p1.x)  |  (seg2.p1.y*seg2.p2.x - seg2.p1.x*seg2.p2.y) */
    delta = (seg1.p1.y - seg1.p2.y)*(seg2.p2.x - seg2.p1.x) - (seg2.p1.y - seg2.p2.y)*(seg1.p2.x - seg1.p1.x);
    /* Проверяем: delta == 0 <=> прямые параллельны (совпадают) */
    if(fabs(delta) < EPS)
        return 0;

    /* Дорешиваем систему уравнений */
    delta_x = (seg1.p1.y*seg1.p2.x - seg1.p1.x*seg1.p2.y)*(seg2.p2.x - seg2.p1.x) - (seg2.p1.y*seg2.p2.x - seg2.p1.x*seg2.p2.y)*(seg1.p2.x - seg1.p1.x);
    delta_y = (seg1.p1.y - seg1.p2.y)*(seg2.p1.y*seg2.p2.x - seg2.p1.x*seg2.p2.y) - (seg2.p1.y - seg2.p2.y)*(seg1.p1.y*seg1.p2.x - seg1.p1.x*seg1.p2.y);

    point_t p = {delta_x / delta, delta_y / delta}; // Искомая точка пересечения
    if(Does_point_belong_to_segment(p, seg1) && Does_point_belong_to_segment(p, seg2))
        return 1;

    return 0;
}



extern geom_validity_t Is_polygon_valid(polygon_t polygon)
{
    assert(polygon.size && polygon.vertices && "Polygon is empty");

    // Меньше 4 вершин - особый случай
    if(polygon.size < 4)
    {
        // Точка
        if(polygon.size == 1)
            return gv_IS_POINT;
        // Отрезок (проверяем совпадение концов)
        if(polygon.size == 2)
            return gv_IS_SEGMENT;

        // Проверяем вырожденность треугольника (с помощью ориентированной площади)
        double  x1 = polygon.vertices[1].x - polygon.vertices[0].x ,
                y1 = polygon.vertices[1].y - polygon.vertices[0].y ,
                x2 = polygon.vertices[2].x - polygon.vertices[0].x ,
                y2 = polygon.vertices[2].y - polygon.vertices[0].y ;
        double S_or = x1*y2 - x2*y1;
        return (fabs(S_or) < EPS ? gv_DEGENERATE : gv_VALID);
    }


    /* Проверка многоугольника ( >= 4 стороны) на самопересечения */
    segment_t tmp_1,
              tmp_2;
    // Проверка последней стороны, которая не вписывается в общий цикл
    tmp_1.p1 = polygon.vertices[0];
    tmp_1.p2 = polygon.vertices[polygon.size - 1];
    for(size_t i = 1; i < polygon.size - 2; ++i)
    {
        tmp_2.p1 = polygon.vertices[i];
        tmp_2.p2 = polygon.vertices[i+1];
        if(Do_segments_intersect(tmp_1, tmp_2)) /* Проверка пересечения */
            return gv_HAS_SELF_INTERSECTION;
    }
    /* Основной цикл, пробегающий по всем остальным сторонам многоугольника */
    for(size_t i = 0; i < polygon.size - 2; ++i)
    {
        tmp_1.p1 = polygon.vertices[i];
        tmp_1.p2 = polygon.vertices[i+1];
        for(size_t j = i+2; j < polygon.size - 1; ++j)
        {
            tmp_2.p1 = polygon.vertices[j];
            tmp_2.p2 = polygon.vertices[j+1];
            if(Do_segments_intersect(tmp_1, tmp_2))
                return gv_HAS_SELF_INTERSECTION;
        }
    }

    return gv_VALID;
}



extern double Calc_dist_bet_point_and_segment(point_t p, segment_t seg)
{
    if(Does_point_belong_to_segment(p, seg))
        return 0.0;

    /* Проверяем, можно ли точку спроецировать на отрезок */
    double len_of_seg = Calc_dist_bet_points(seg.p1, seg.p2);
    double scalar_prod =  SCALAR_PRODUCT(seg.p1, p, seg.p1, seg.p2) / len_of_seg;
    if((scalar_prod >= 0) && (scalar_prod <= len_of_seg))
    {
        /* Можно спроецировать => расстояние от точки до прямой равно расстоянию до отрезка: */
        // Подставим точку в ур-ние прямой:
        double eq_val = (seg.p1.y - seg.p2.y)*p.x + (seg.p2.x - seg.p1.x)*p.y + (seg.p1.x*seg.p2.y - seg.p1.y*seg.p2.x);
        // Посчитаем длину нормали прямой sqrt(A^2 + B^2):
        double n_len = hypot((seg.p1.y - seg.p2.y), (seg.p2.x - seg.p1.x));
        // sqrt((seg.p1.y - seg.p2.y)*(seg.p1.y - seg.p2.y) + (seg.p2.x - seg.p1.x)*(seg.p2.x - seg.p1.x));
        return fabs(eq_val / n_len);
    }

    // Иначе выбираем меньшее из расстояний до концов
    return fmin(Calc_dist_bet_points(p, seg.p1), Calc_dist_bet_points(p, seg.p2));
}



extern double Calc_dist_bet_segments(segment_t seg_1, segment_t seg_2)
{
    if(Do_segments_intersect(seg_1, seg_2)) // Проверка пересечения отрезков
        return 0.0;

    /* Далее сравниваем расстояния от каждого конца 1-го отрезка до 2-го отрезка и наоборот*/
    double min_dist_1 = fmin(Calc_dist_bet_point_and_segment(seg_1.p1, seg_2), Calc_dist_bet_point_and_segment(seg_1.p2, seg_2)),
           min_dist_2 = fmin(Calc_dist_bet_point_and_segment(seg_2.p1, seg_1), Calc_dist_bet_point_and_segment(seg_2.p2, seg_1));

    return fmin(min_dist_1, min_dist_2);
}



extern double Calc_dist_bet_polygons(polygon_t pol_1, polygon_t pol_2, geom_validity_t * gv_err)
{
    assert(pol_1.vertices && pol_1.size && "First polygon is empty"); //REMOVE
    assert(pol_2.vertices && pol_2.size && "Second polygon is empty"); //REMOVE
    //assert((pol_1.size > 2) && (pol_2.size > 2) && "Polygon(s) has less vertices than expexted"); //REMOVE
    if((pol_1.size < 3) || (pol_2.size < 3))
    {
        *gv_err = gv_NOT_POLYGON;
        return 0.0;
    }

    /* Пройдём по всем парам сторон многоугольников, считая расстояния межу ними */
    segment_t side_of_pol_1 = {pol_1.vertices[0], pol_1.vertices[pol_1.size - 1]},
              side_of_pol_2 = {pol_2.vertices[0], pol_2.vertices[pol_2.size - 1]};
    double min_dist = Calc_dist_bet_segments(side_of_pol_1, side_of_pol_2);

    /* Отдельно, перебирая все, окромя последней, стороны 2-ого многоугольника,
        обработаем последнюю сторону 1-ого мн-ка, т.к. она не вписывается в основной цикл */
    for(size_t j = 0; j < pol_2.size - 1; ++j)
    {
        side_of_pol_2.p1 = pol_2.vertices[j];
        side_of_pol_2.p2 = pol_2.vertices[j+1];
        min_dist = fmin(min_dist, Calc_dist_bet_segments(side_of_pol_1, side_of_pol_2));
        if(min_dist < EPS) /* Проверка на пересечение сторон */
            return 0.0;
    }

    /* Основной цикл: перебираем все остальные пары сторон мн-ков */
    for(size_t i = 0; i < pol_1.size - 1; ++i)
    {
        side_of_pol_1.p1 = pol_1.vertices[i];
        side_of_pol_1.p2 = pol_1.vertices[i+1];

        /* Отдельная обработка последней, выбивающейся из цикла, стороны */
        side_of_pol_2.p1 = pol_2.vertices[0];
        side_of_pol_2.p2 = pol_2.vertices[pol_2.size - 1];
        min_dist = fmin(min_dist, Calc_dist_bet_segments(side_of_pol_1, side_of_pol_2));
        if(min_dist < EPS) /* Проверка на пересечение многоугольников */
            return 0.0;

        /* Для фиксированной стороны 1-ого мн-ка перебираем все (кроме последней) стороны 2-ого */
        for(size_t j = 0; j < pol_2.size - 1; ++j)
        {
            side_of_pol_2.p1 = pol_2.vertices[j];
            side_of_pol_2.p2 = pol_2.vertices[j+1];
            min_dist = fmin(min_dist, Calc_dist_bet_segments(side_of_pol_1, side_of_pol_2));
            if(min_dist < EPS) /* Проверка на пересечение многоугольников */
                return 0.0;
        }
    }

    assert((min_dist >= 0.) && "Negative value of distance"); //REMOVE

    return min_dist;
}



static inline double Calc_squared_dist_bet_points(point_t p1, point_t p2)
{ return ((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)); }

#include <stdlib.h>
#include <stdio.h>
#include "../geom_lib/header/geom_objects.h"
#include "../geom_lib/header/reading_objects_from_file.h"
#include "../geom_lib/header/algos.h"


/* Функция из файла inp считывает многоугольник в polygon.
   Возращает код ошибки прочтения многоугольника. */
static reading_code_t Read_and_check_polygon(FILE *inp, polygon_t * polygon);


extern double Read_and_calculate_distance(FILE *inp, reading_code_t *err)
{
    polygon_t polygon_1 = {0, NULL},
              polygon_2 = {0, NULL};

    /* Считываем первый многоугольник и проверяем успешность этой операции. */
    *err = Read_and_check_polygon(inp, &polygon_1);
    if(*err != rc_SUCCESFULLY_READ)
    {
        if(polygon_1.vertices != NULL)
            free(polygon_1.vertices);
        return 0.0;
    }
    /* Считываем второй многоугольник и проверяем успешность этой операции. */
    *err = Read_and_check_polygon(inp, &polygon_2);
    if(*err != rc_SUCCESFULLY_READ)
    {
        free(polygon_1.vertices);
        if(polygon_2.vertices != NULL)
            free(polygon_2.vertices);
        return 0.0;
    }

    /* Рассчёт расстояния между прочитанными многоугольниками */
    geom_validity_t gv_err = gv_VALID;
    double distance = Calc_dist_bet_polygons(polygon_1, polygon_2, &gv_err);
    if(gv_err == gv_NOT_POLYGON)
        *err = rc_NOT_POLYGON;

    free(polygon_1.vertices);
    free(polygon_2.vertices);
    return distance;
}


static reading_code_t Read_and_check_polygon(FILE *inp, polygon_t * polygon)
{
    reading_code_t err = Read_polygon_from_file(inp, polygon);
    geom_validity_t gv_err;

    if(err == rc_SUCCESFULLY_READ)
    {
        gv_err = Is_polygon_valid(*polygon);
        if(gv_err == gv_VALID)
            return rc_SUCCESFULLY_READ;
        else if(gv_err == gv_HAS_SELF_INTERSECTION)
            return rc_POLYGON_HAS_SELF_INTERSECTION;
        else
            return rc_UNIDENTIFIED_ERROR;
    }

    return err;
}

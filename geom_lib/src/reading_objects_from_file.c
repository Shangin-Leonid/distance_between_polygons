#include <stdlib.h>
#include <stdio.h>
//#define NDEBUG // Отключает использование макросов assert() //REMOVE: расскоментить
#include <assert.h>
#include "../header/geom_objects.h"
#include "../header/reading_objects_from_file.h"



extern reading_code_t Read_polygon_from_file(FILE * inp, polygon_t * polygon)
{
    assert(inp &&
            "Pointer to input file is empty. Error of calling function.");
    assert((polygon->vertices == NULL) &&
            "Pointer to array of vertices is not empty. Inputing these data to the function is not safeful.");

    reading_code_t status_of_reading = rc_SUCCESFULLY_READ; // Код ошибки прочтения точки
    size_t amount_of_vertices; // Количество вершин многоугольника (его размер)
    int tmp_i, // Буфер для считывания количества вершин и проверки на положительность
        status_of_fscanf; // Код ошибки работы 'fscanf'
    char tmp_c; // char-буфер

    /* Считываем открывающую многоугольник скобку */
    status_of_fscanf = fscanf(inp, " %c", &tmp_c);
    if(status_of_fscanf == EOF)
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    else if((status_of_fscanf != 1) || (tmp_c != '<'))
        return rc_INCORRECT_INPUT_DATA;

    /* Считываем количество вершин многоугольника */
    status_of_fscanf = fscanf(inp, " %d", &tmp_i);
    if(status_of_fscanf == EOF)
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    else if((status_of_fscanf != 1) || (tmp_i <= 0))
        return rc_INCORRECT_INPUT_DATA;

    /* Запоминаем количество вершин и выделяем память для массива вершин */
    amount_of_vertices = tmp_i;
    polygon->vertices = (point_t *) malloc(amount_of_vertices * sizeof(point_t));
    if(polygon->vertices == NULL)
        return rc_MEMORY_ALLOCATING_ERROR;

    /* В цикле считываем все вершины */
    for(size_t i = 0; i < amount_of_vertices; i++)
    {
        status_of_reading = Read_point_from_file(inp, polygon->vertices + i);
        if(status_of_reading != rc_SUCCESFULLY_READ)
        {
            free(polygon->vertices);
            polygon->vertices = NULL;
            return (status_of_reading == rc_FILE_ENDED_EARLIER_THAN_EXPECTED ? rc_FILE_ENDED_EARLIER_THAN_EXPECTED : rc_INCORRECT_INPUT_DATA);
        }
    }

    /* Считываем закрывающую многоугольник скобку */
    status_of_fscanf = fscanf(inp, " %c", &tmp_c);
    if(status_of_fscanf == EOF){
        free(polygon->vertices);
        polygon->vertices = NULL;
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    }
    else if((status_of_fscanf != 1) || (tmp_c != '>')){
        free(polygon->vertices);
        polygon->vertices = NULL;
        return rc_INCORRECT_INPUT_DATA;
    }

    /* Не забываем указать размер */
    polygon->size = amount_of_vertices;
    return rc_SUCCESFULLY_READ;
}


extern reading_code_t Read_point_from_file(FILE * inp, point_t * p)
{
    assert(inp && "Pointer to input file is empty"); // inp == NULL - ошибка вызывающей функции

    int status_of_fscanf;
    char tmp_c;

    /* Считываем первую (открывающую) фигурную скобку для координат читаемой точки */
    status_of_fscanf = fscanf(inp, " %c", &tmp_c);
    if(status_of_fscanf == EOF)
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    else if((status_of_fscanf != 1) || (tmp_c != '{'))
        return rc_INCORRECT_INPUT_DATA;

    /* Считываем первую координату точки  */
    status_of_fscanf = fscanf(inp, " %lf", &(p->x));
    if(status_of_fscanf == EOF)
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    else if(status_of_fscanf != 1)
        return rc_INCORRECT_INPUT_DATA;

    /* Считываем разделитльную точку с запятой */
    status_of_fscanf = fscanf(inp, " %c", &tmp_c);
    if(status_of_fscanf == EOF)
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    else if((status_of_fscanf != 1) || (tmp_c != ';'))
        return rc_INCORRECT_INPUT_DATA;

    /* Считываем вторую координату точки  */
    status_of_fscanf = fscanf(inp, " %lf", &(p->y));
    if(status_of_fscanf == EOF)
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    else if(status_of_fscanf != 1)
        return rc_INCORRECT_INPUT_DATA;

    /* Считываем вторую (закрывающую) фигурную скобку для координат читаемой точки */
    status_of_fscanf = fscanf(inp, " %c", &tmp_c);
    if(status_of_fscanf == EOF)
        return rc_FILE_ENDED_EARLIER_THAN_EXPECTED;
    else if((status_of_fscanf != 1) || (tmp_c != '}'))
        return rc_INCORRECT_INPUT_DATA;

    return rc_SUCCESFULLY_READ;
}



extern void Print_reading_error_to_file(FILE * outp, reading_code_t status)
{
    if(outp == NULL)
    {
        switch(status)
        {
            case rc_SUCCESFULLY_READ:
                //printf("Success!\n"); //REMOVE: only for checking
                break;
            case rc_END_OF_FILE:
                printf("END_OF_FILE!\n");
                break;
            case rc_INCORRECT_INPUT_DATA:
                printf("INCORRECT_INPUT_DATA!\n");
                break;
            case rc_MEMORY_ALLOCATING_ERROR:
                printf("MEMORY_ALLOCATING_ERROR!\n");
                break;
            case rc_FILE_ENDED_EARLIER_THAN_EXPECTED:
                printf("FILE_ENDED_EARLIER_THAN_EXPECTED!\n");
                break;
            case rc_POLYGON_HAS_SELF_INTERSECTION:
                printf("ONE OF POLYGONS HAS SELF INTERSECTION!\n");
                break;
            case rc_NOT_POLYGON:
                printf("NOT_POLYGON!\n");
                break;
            case rc_UNIDENTIFIED_ERROR:
                printf("UNIDENTIFIED_ERROR!\n");
                break;
        }
    }

    else
    {
        switch(status)
        {
            case rc_SUCCESFULLY_READ:
                //fprintf(outp, "Success!\n"); //REMOVE: only for checking
                break;
            case rc_END_OF_FILE:
                fprintf(outp, "END_OF_FILE!\n");
                break;
            case rc_INCORRECT_INPUT_DATA:
                fprintf(outp, "INCORRECT_INPUT_DATA!\n");
                break;
            case rc_MEMORY_ALLOCATING_ERROR:
                fprintf(outp, "MEMORY_ALLOCATING_ERROR!\n");
                break;
            case rc_FILE_ENDED_EARLIER_THAN_EXPECTED:
                fprintf(outp, "FILE_ENDED_EARLIER_THAN_EXPECTED!\n");
                break;
            case rc_POLYGON_HAS_SELF_INTERSECTION:
                fprintf(outp, "ONE OF POLYGONS HAS SELF INTERSECTION!\n");
                break;
            case rc_NOT_POLYGON:
                fprintf(outp, "NOT_POLYGON!\n");
                break;
            case rc_UNIDENTIFIED_ERROR:
                fprintf(outp, "UNIDENTIFIED_ERROR!\n");
                break;
        }
    }
}

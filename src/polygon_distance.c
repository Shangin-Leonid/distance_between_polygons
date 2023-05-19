#include <stdlib.h>
#include <stdio.h>
#include "../geom_lib/header/reading_objects_from_file.h"
#include "../header/reading_and_calculating.h"


/* Коды ошибок main функции */
enum MAIN_RETURN_VALUES{
    mr_OK = 0,
    mr_NO_ARGUMENTS_PASSED = 1,
    mr_TOO_MANY_ARGUMENTS_PASSED = 2,
    mr_OPENNING_INPUT_FILE_ERROR = 3,
    mr_OPENNING_OUTPUT_FILE_ERROR = 4
};


int main(int argc, char* argv[])
{

    /* Проверяем количество переданных на вход аргументов */
    if((argc == 1) || (argv == NULL))
    {
        printf("\nInput file hasn't been passed to main function (no arguments have been passed).\n");
        return mr_NO_ARGUMENTS_PASSED;
    }
    else if(argc > 3)
    {
        printf("\nToo many arguments (%d) have been passed to main function.\n", argc-1);
        return mr_TOO_MANY_ARGUMENTS_PASSED;
    }


    FILE *inp = NULL,  // Входной файл
         *outp = NULL; // Выходной файл
    reading_code_t err; // Статус прочтения геометрического объекта из файла

    /* Если получили имя только входного файла */
    if(argc == 2)
    {
        /* Открываем входной файл и проверяем успешность */
        inp = fopen(argv[1], "r");
        if(inp == NULL)
        {
            printf("\nSome problems with opening input file \"%s\".\n", argv[1]);
            return mr_OPENNING_INPUT_FILE_ERROR;
        }

        /* Входной файл открыт - выдаём предупреждение и далее запускаем рассчёты и вывод результатов */
        printf("\nBe careful: you have passed only input file name.\n");
        printf("Try again with adding output file name to get more info about results.\n");

        printf("\nThe programm is finding the distance between two polygons from file \"%s\".\n", argv[1]);
        printf("The result you can see in the console. Please wait...\n\n");

        double distance = Read_and_calculate_distance(inp, &err);
        if(err == rc_SUCCESFULLY_READ)
            printf("The distance is %.8lf", distance);
        else
            printf("Error. More information after rerunning the program with output file name as one of arguments.\n");

        fclose(inp);
        return mr_OK;
    }


    /* Открываем входной и выходной файлы, проверяя ошибки открытий */
    outp = fopen(argv[2], "w");
    if(outp == NULL)
    {
        printf("\nSome problems with opening %s.\n", argv[2]);
        return mr_OPENNING_OUTPUT_FILE_ERROR;
    }
    inp = fopen(argv[1], "r");
    if(inp == NULL)
    {
        fprintf(outp, "\nSome problems with opening \"%s\".\n", argv[1]);
        fclose(outp);
        return mr_OPENNING_INPUT_FILE_ERROR;
    }

    /* Файлы открыты - читаем многоугольники и рассчитываем расстояние. */
    printf("\nThe programm is finding the distance between two polygons from file \"%s\".\n", argv[1]);
    printf("The result you can see in \"%s\". Please wait...\n\n", argv[2]);

    double distance = Read_and_calculate_distance(inp, &err);
    if(err == rc_SUCCESFULLY_READ)
    {
        printf("The distance is %.8lf", distance);
        fprintf(outp, "The distance is %.8lf\n", distance);
    }
    else
    {
        printf("Error. More information in \"%s\".\n", argv[2]);
        Print_reading_error_to_file(outp, err);
    }

    fclose(inp);
    fclose(outp);
    return mr_OK;
}

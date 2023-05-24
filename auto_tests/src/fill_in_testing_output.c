#include <stdlib.h>
#include <stdio.h>
#include "../../header/reading_and_calculating.h"
#include "../../geom_lib/header/reading_objects_from_file.h"
#include "../header/names_of_files_and_dirs.h"



int main(int argc, char* argv[])
{
    FILE *inp = NULL, // Входной файл
         *outp = NULL; // Выходной файл
    reading_code_t err; // Статус прочтения из файла геометрического объекта

    /* Открываем файл на вывод, проверяя ошибки открытия. */
    outp = fopen(NAME_OF_TEST_OUTPUT_FILE, "w");
    if(outp == NULL)
    {
        printf("Some problems with opening %s.", NAME_OF_TEST_OUTPUT_FILE);
        return -1;
    }
    /* Если не передали никаких аргументов */
    if((argc == 1) || (argv == NULL))
    {
        fprintf(outp, "F Testing function did't get any argument (name of input file).\n");
        printf("Testing function did't get any argument (name of input file).\n");
        fclose(outp);
        return 1;
    }

    for(int i = 1; i < argc; ++i)
    {
        /* Открываем очередной входой файл, проверяя ошибки открытия. */
        inp = fopen(argv[i], "r");
        if(inp == NULL)
        {
            fprintf(outp, "F \"%s\": problems with opening this file.\n", argv[i]);
            printf("\"%s\": problems with opening this file.\n", argv[i]);
            continue; // ВАЖНОЕ МЕСТО: в случае ошибки открытия, продолжаем открывать следующие файлы
        }

        // Тестируем программу на текущем файле
        double distance = Read_and_calculate_distance(inp, &err);
        //Результат теста выводим в файл в соответствующем формате
        if(err == rc_SUCCESFULLY_READ)
            fprintf(outp, "S %.8lf\n", distance);
        else
            fprintf(outp, "E %d\n", err);

        fclose(inp);
    }

    fclose(outp);
    return 0;
}

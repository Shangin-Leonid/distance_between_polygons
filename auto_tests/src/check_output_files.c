#include <stdlib.h>
#include <stdio.h>
#include "../header/names_of_files_and_dirs.h"


#define DELTA 1e-6


/* Функция модуля числа */
static inline double dabs(double n)
{ return ((n >= 0.) ? n : -n); }



int main(int argc, char* argv[])
{
    FILE *routp, // Файл с результатами
         *toutp; // Файл с выводом тестов

    /* Поочерёдно открываем все файлы */
    routp = fopen(NAME_OF_TEST_REPORT_FILE, "w");
    if(routp == NULL)
    {
        printf("Problems with opening \"%s\".\n", NAME_OF_TEST_REPORT_FILE);
        return -1;
    }
    // Выводим в файл результатов заголовок
    fprintf(routp, "\n\tHere you can see results of testing the programm:\n\n");

    if((argc == 1) || (argv == NULL))
    {
        printf("\n\nChecking function did't get any argument (name of file with correct answer).\n");
        fprintf(routp, "\n\nChecking function did't get any argument (name of file with correct answer).\n");
        fclose(routp);
        return 1;
    }

    toutp = fopen(NAME_OF_TEST_OUTPUT_FILE, "r");
    if(toutp == NULL)
    {
        printf("\n\nThere is no \"%s\". You should run \"%s\"", NAME_OF_TEST_OUTPUT_FILE, NAME_OF_FILL_IN_PROGRAMM);
        printf(" before running this checking programm\n");
        fprintf(routp, "\n\nThere is no \"%s\". You should run \"%s\"", NAME_OF_TEST_OUTPUT_FILE, NAME_OF_FILL_IN_PROGRAMM);
        fprintf(routp, " before running this checking programm\n");
        fclose(routp);
        return 1;
    }

    /* В цикле сверяем правильные выходные данные с теми, которые лежат в FILE *toutp */
    for(int i = 1; i < argc; ++i)
    {
        char out_symbol = ' ',
             correct_symbol = ' ';
        int status_of_fscanf;
        int correct_code,
            test_code;
        double correct_dist = 0.0,
               test_dist = 0.0;

        FILE * coutp = fopen(argv[i], "r"); // Файл с текущими верными ответами
        if(coutp == NULL)
        {
            fprintf(routp, "> \"%s\": UNDONE. Problem with opening answer-file. If there is no such file, please ADD IT.\n", argv[i]);
            continue;
        }

        /* Считываем первую букву выходных данных программы, чтобы узнать, какого-рода данные находятся дальше */
        status_of_fscanf = fscanf(toutp, " %c", &out_symbol);
        /* Случай, когда не удалось протестировать файл, соответствующий обрабатываемому сейчас ответу */
        if((out_symbol != 'S') && (out_symbol != 'E'))
        {
            /* Случай, когда не удалось открыть файл с входными данными при формировании файла с результатами работы программы */
            if(out_symbol == 'F')
                fprintf(routp, "> \"%s%02d.txt\": UNDONE. Were problems with opening \"%s%02d.txt\" during doing tests.\n", CASE_PREFIX, i, CASE_PREFIX, i);
            /* Случай, когда в FILE *toutp некорректные для теста данные */
            else
            {
                fprintf(routp, "> \"%s%02d.txt\": UNDONE. Incorrect data in \"%s\".\n", CASE_PREFIX, i, NAME_OF_TEST_OUTPUT_FILE);
                fprintf(routp, "                         Maybe there is no \"%s/%s%02d.txt\".\n", TEST_CASES_DIR, CASE_PREFIX, i);
            }

            fclose(coutp);
            continue;
        }

        /* Считываем первую букву файла с ответами, чтобы узнать, какого-рода данные находятся дальше */
        status_of_fscanf = fscanf(coutp, " %c", &correct_symbol);
        /* Случай, когда данные в файле с ответами некорректны */
        if((correct_symbol != 'S') && (correct_symbol != 'E'))
        {
            fprintf(routp, "> \"%s%02d.txt\": UNDONE. Incorrect data in \"%s\".\n", CASE_PREFIX, i, argv[i]);

            fclose(coutp);
            continue;
        }

        /* Сравниваем первые буквы из файла с выходными данными программы и файла с правильными ответами:
            Если не совпали, то один файл указывает на ошибку, а второй подразумевает, что расстояние было посчитано */
        if(out_symbol != correct_symbol)
        {
            if(out_symbol == 'S')
            {
                fprintf(routp, "> \"%s%02d.txt\": FAILED. Programm did't catch an error of input data.\n", CASE_PREFIX, i);
                fscanf(toutp, " %lf", &test_dist);
                if(correct_symbol == 'E')
                    fscanf(coutp, " %d", &correct_code);
            }
            else
            {
                fprintf(routp, "> \"%s%02d.txt\": FAILED. An error during programm working, while there is no problem in truth.\n", CASE_PREFIX, i);
                fscanf(coutp, " %lf", &correct_dist);
                if(out_symbol == 'E')
                    fscanf(toutp, " %d", &test_code);
            }

            fclose(coutp);
            continue;
        }

        /* Случай, когда входные данные были корректны - в FILE *toutp было записано расстояние */
        if(out_symbol == 'S')
        {
            /* Считываем расстояния из файла с результатами работы программы и из файла с ответами */
            status_of_fscanf = fscanf(coutp, " %lf", &correct_dist);
            if((status_of_fscanf != 1) || (correct_dist < 0))
            {
                fprintf(routp, "> \"%s%02d.txt\": UNDONE. INCORRECT DATA IN FILE \"%s\". FIX IT.\n", CASE_PREFIX, i, argv[i]);

                fclose(coutp);
                continue;
            }
            status_of_fscanf = fscanf(toutp, " %lf", &test_dist);
            if(status_of_fscanf != 1)
            {
                fprintf(routp, "> \"%s%02d.txt\": UNDONE. INCORRECT DATA IN TESTING OUTPUT FILE \"%s\". FIX IT.\n", CASE_PREFIX, i, NAME_OF_TEST_OUTPUT_FILE);

                fclose(coutp);
                continue;
            }

            /* Теперь сравним результат работы программы с верным ответом */
            if(dabs(test_dist - correct_dist) <= DELTA*correct_dist)
                fprintf(routp, "> \"%s%02d.txt\": PASSED\n", CASE_PREFIX, i);
            else
                fprintf(routp, "> \"%s%02d.txt\": FAILED. Programm output is '%lf', while correct is '%lf'\n", CASE_PREFIX, i, test_dist, correct_dist);

            fclose(coutp);
            continue;
        }


        /* Случай, когда программа выдала ошибку - в FILE *toutp был записан код ошибки */
        else if(out_symbol == 'E')
        {
            /* Считываем расстояния из файла с результатами работы программы и из файла с ответами */
            status_of_fscanf = fscanf(coutp, " %d", &correct_code);
            if(status_of_fscanf != 1)
            {
                fprintf(routp, "> \"%s%02d.txt\": UNDONE. INCORRECT DATA IN FILE \"%s\". FIX IT.\n", CASE_PREFIX, i, argv[i]);

                fclose(coutp);
                continue;
            }
            status_of_fscanf = fscanf(toutp, " %02d", &test_code);
            if(status_of_fscanf != 1)
            {
                fprintf(routp, "> \"%s%02d.txt\": UNDONE. INCORRECT DATA IN TESTING OUTPUT FILE \"%s\". FIX IT.\n", CASE_PREFIX, i, NAME_OF_TEST_OUTPUT_FILE);

                fclose(coutp);
                continue;
            }

            /* Теперь сравним результат работы программы с верным ответом */
            if(correct_code == test_code)
                fprintf(routp, "> \"%s%02d.txt\": PASSED\n", CASE_PREFIX, i);
            else
                fprintf(routp, "> \"%s%02d.txt\": FAILED. Programm error code is '%d', while correct is '%d'\n", CASE_PREFIX, i, test_code, correct_code);

            fclose(coutp);
            continue;
        }

        fclose(coutp);
    }

    fclose(toutp);
    fclose(routp);
    return 0;
}

// Copyright 2024 Vladislav Aleinik
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>

#include <time.h>

//========================================//
// Макрос для удобной проверки контрактов //
//========================================//

// Для применения макроса VERIFY_CONTRACT требуется
// подключение библиотеки stdlib.h.
#define VERIFY_CONTRACT(contract, format, ...) \
    do {                                       \
        if (!(contract)) {                     \
            printf((format), ##__VA_ARGS__);   \
            exit(EXIT_FAILURE);                \
        }                                      \
    } while (0)

//============================//
// Алгоритмы поиска подстроки //
//============================//

//==================================================================================================
// Функция: strstr_naive
// Назначение: производит поиск подстроки в строке
//--------------------------------------------------------------------------------------------------
// Параметры:
// haystack (in) - исходная строка, в которой производится поиск.
// needle (in) - искомая строка.
//
// Возвращаемое значение:
// Указатель на фрагамент внутри строки haystack, равный строке needle, или NULL,
// если подстрока needle не содержится в строке haystack.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Алгоритм не является оптимальным.
//==================================================================================================
const char* strstr_naive(const char* haystack, const char* needle)
{
    // Вычисляем размеры обеих строк:
    size_t haystack_len = strlen(haystack);
    size_t needle_len   = strlen(needle);

    if (needle_len > haystack_len)
    {
        // Поиск бОльшей подстроки в меньшей результата не даст
        return NULL;
    }

    // Полный перебор всех начальных позиций внутри строки haystack
    const char* cur = haystack;
    while (cur + needle_len <= haystack + haystack_len)
    {
        // Посимвольно проверяем факт совпадения фрагмента строки с искомой строкой
        bool match = true;
        for (size_t index = 0U; index < needle_len; ++index)
        {
            if (cur[index] != needle[index])
            {
                match = false;
                break;
            }
        }

        // Возвращаем первое совпадение
        if (match)
        {
            return cur;
        }

        // При несоответствии фрагмента переходим к следующему кандидату
        cur += 1;
    }

    // Искомая строка не содержится в исходной строке
    return NULL;
}

//==================================================================================================
// Функция: strstr_rabin_karp
// Назначение: производит поиск подстроки в строке
//--------------------------------------------------------------------------------------------------
// Параметры:
// haystack (in) - исходная строка, в которой производится поиск.
// needle (in) - искомая строка.
//
// Возвращаемое значение:
// Указатель на фрагамент внутри строки haystack, равный строке needle, или NULL,
// если подстрока needle не содержится в строке haystack.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
const char* strstr_rabin_karp(const char* haystack, const char* needle)
{
    // Длина искомой строки
    size_t needle_len = 0;

    // Вычисление хеша от искомой строки
    uint32_t needle_hash = 0;
    for (const char* needle_ptr = needle; *needle_ptr != '\0'; ++needle_ptr, ++needle_len)
    {
        needle_hash += *needle_ptr;
    }

    // Вычисление хеша от исходной строки
    uint32_t haystack_hash = 0;
    for (size_t haystack_i = 0; haystack_i < needle_len; ++haystack_i)
    {
        if (haystack[haystack_i] == '\0')
        {
            // Искомая строка больше исходной
            return NULL;
        }

        haystack_hash += haystack[haystack_i];
    }

    // Поиск фрагмента с совпадающим хэшом
    for (const char* haystack_ptr = haystack + needle_len; true; ++haystack_ptr)
    {
        // Проверка совпадения хэша и полная проверка равенства строк
        if (haystack_hash == needle_hash &&
            strncmp(needle, haystack_ptr - needle_len, needle_len) == 0)
        {
            // Искомая строка найдена
            return haystack_ptr - needle_len;
        }

        if (*haystack_ptr == '\0')
        {
            return NULL;
        }

        // Обновление хеша от фрагмента искомой строки
        haystack_hash += *haystack_ptr - *(haystack_ptr - needle_len);
    }

    // Недостижимый фрагмент кода
}

//==================================================================================================
// Функция: strstr_knuth_morris_pratt
// Назначение: производит поиск подстроки в строке
//--------------------------------------------------------------------------------------------------
// Параметры:
// haystack (in) - исходная строка, в которой производится поиск.
// needle (in) - искомая строка.
//
// Возвращаемое значение:
// Указатель на фрагамент внутри строки haystack, равный строке needle, или NULL,
// если подстрока needle не содержится в строке haystack.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// См. Cormen, T. H.; Leiserson, C. E.; Rivest, R. L. & Stein, C. (2009),
// Introduction to Algorithms, The MIT Press, изд. 3, стр. 1005.
//==================================================================================================
const char* strstr_knuth_morris_pratt(const char* haystack, const char* needle)
{
    // Этап I: вычисление префикс-функции

    // Вычисление длины искомой строки за O(m)
    size_t needle_len = strlen(needle);

    // Префикс-функция искомой строки
    // Префикс-функция[i] - это максимальный размер префикса, который при этом является
    //                      суффиксом и не совпадает с самой строкой.
    size_t prefix_function[needle_len + 1];

    // Префикс функция для префикса длины 1 равна 0
    prefix_function[1] = 0;

    // Потенциальное значение префикс-функции
    size_t candidate = 0;

    // Вычисление префикс-функции
    for (size_t prefix_len = 2; needle[prefix_len] != '\0'; ++prefix_len)
    {
        // Поиск префикса, который можно расширить так, чтобы он совпадал с текущим суффиксом
        while (candidate > 0 && needle[candidate] != needle[prefix_len - 1])
        {
            // Используется свойство:
            // Если подстрока длины n имеет префикс-суффикс длины m (m < n),
            //    а подстрока длины m имеет префикс-суффикс длины k (k < m),
            //    то подсторка длины n имеет префикс-суффикс длины k:
            // Иллюстрация свойства:
            // abacaba
            // aba aba
            // a a a a
            // a     a
            candidate = prefix_function[candidate];
        }

        if (needle[candidate] == needle[prefix_len - 1])
        {
            // В случае совпадения нового символа префикс можно расширить
            candidate += 1;
        }

        prefix_function[prefix_len] = candidate;
    }

    // Этап II: поиск вхождения подстроки

    // Потенциальное значение префикс-функцииы
    candidate = 0;

    // Вычисление префикс-функции для исходной строки
    for (const char* haystack_ptr = haystack; *haystack_ptr != '\0'; haystack_ptr++)
    {
        // Поиск префикса, который можно расширить так, чтобы он совпадал с текущим суффиксом
        while (candidate > 0 && needle[candidate] != *haystack_ptr)
        {
            candidate = prefix_function[candidate];
        }

        if (needle[candidate] == *haystack_ptr)
        {
            // В случае совпадения нового символа префикс можно расширить
            candidate += 1;
        }

        if (candidate == needle_len)
        {
            return haystack_ptr - (needle_len - 1);
        }
    }

    return NULL;
}

//=============================//
// Главная процедура программы //
//=============================//

// Имя файла, в котором производится поиск
const char* HAYSTACK_FILENAME = "res/warandpeace.txt";

#define NUM_ITERATIONS 10

int main(void)
{
    //------------------------//
    // Чтение строки из файла //
    //------------------------//

    // Открываем файл
    FILE* haystack_file = fopen(HAYSTACK_FILENAME, "r");
    VERIFY_CONTRACT(
        haystack_file != NULL,
        "Unable to open file: %s\n", HAYSTACK_FILENAME
    );

    // Измеряем размер файла
    int ret = fseek(haystack_file, 0L, SEEK_END);
    VERIFY_CONTRACT(
        ret != -1,
        "Unable to find end of file: %s\n", HAYSTACK_FILENAME
    );

    long haystack_len = ftell(haystack_file);
    VERIFY_CONTRACT(
        haystack_len != -1,
        "Unable to measure size for file: %s\n", HAYSTACK_FILENAME
    );

    // Возвращаем каретку на исходную точку
    ret = fseek(haystack_file, 0L, SEEK_SET);
    VERIFY_CONTRACT(
        ret != -1,
        "Unable to rewind to start of file: %s\n", HAYSTACK_FILENAME
    );

    //--------------------------------//
    // Подготавливаем исходную строку //
    //--------------------------------//

    // Выделяем место для исходной строки
    // Добавляем +1 для символа конца строки
    char* haystack = (char*) malloc(haystack_len + 1);
    VERIFY_CONTRACT(
        haystack != NULL,
        "Unable to allocate %ld bytes of memory\n", haystack_len
    );

    // Считывеам строку из файла:
    size_t bytes_read = fread(haystack, 1U, haystack_len, haystack_file);
    VERIFY_CONTRACT(
        bytes_read == haystack_len,
        "Unable to read haystack from file (read %lu of %lu bytes)\n",
        bytes_read, haystack_len
    );

    // Выставляем символ конца строки
    haystack[haystack_len] = '\0';

    // Закрываем файл
    ret = fclose(haystack_file);
    VERIFY_CONTRACT(ret == 0,
        "Unable to close file \'%s\'\n",
        HAYSTACK_FILENAME);

    //----------------------------------//
    // Подготовка массива искомых строк //
    //----------------------------------//

    // Оценку производительности алгоритмов поиска строк целесообрано оценивать
    // совместно с корпусом слов, на котором алгоритм будет работать.
    const char* needles[] =
    {
        "любовь", "счастье", "добродетель", "гнев", "смерть",
        "время", "память", "слово", "дело", "смысл", "разум",
        "Андрей", "Наташа", "Николай", "Соня", "Пьер", "Элен", "дуэль",
        "салон", "madame", "mon cher", "обман",
        "Россия", "война", "Кутузов", "Наполеон", "друг", "враг", "гвардия", "бог",
        "есть только две добродетели: деятельность и ум",
        "есть только два источника людских пороков: праздность и суеверие",
        "Нездоровы, брат, бывают только дураки да развратники",
    };

    //---------------------------------------------------------------//
    // Сравнение алгоритмов поиска подстроки на корпусе Войны и Мира //
    //---------------------------------------------------------------//

    // Функции алгоритмов поиска подстроки
    const char* (*algorithms[])(const char*, const char*) =
    {
        &strstr_naive,
        &strstr_rabin_karp,
        &strstr_knuth_morris_pratt,
        (const char* (*)(const char*, const char*)) &strstr
    };

    // Названия алгоритмов поиска подстроки
    const char* algorithm_names[] =
    {
        "Наивный алгоритм",
        "Алгоритм Рабина-Карпа",
        "Алгоритм Кнута-Морриса-Пратта",
        "Библиотечная реализация strstr"
    };

    for (unsigned alg_i = 0; alg_i < 4; ++alg_i)
    {
        // Начало измеряемого отрезка времени
        clock_t ticks_start = clock();

        printf("%.60s\n", algorithm_names[alg_i]);

        for (size_t word_i = 0; word_i < sizeof(needles) / sizeof(const char*); ++word_i)
        {
            // Исползуем word_i-ое слово из корпуса
            const char* needle = needles[word_i];

            // Количество вхождений.
            size_t num_matches = 0;

            for (size_t iterations = 0; iterations < NUM_ITERATIONS; ++iterations)
            {
                const char* cur = haystack;
                while (true)
                {
                    // Ищем следующее вхождение искомой строки с использованием alg_i-го алгоритма
                    cur = algorithms[alg_i](cur, needle);
                    if (cur == NULL)
                    {
                        break;
                    }

                    // Готовимся к поиску следующего вхождения
                    cur += 1;
                    // Учитываем вхождение.
                    num_matches++;
                }
            }

            printf("                                                                      - %zu\r%.120s\n", num_matches/NUM_ITERATIONS, needle);
        }

        // Конец измеряемого отрезка времени
        clock_t ticks_end = clock();

        double ticks_delta = ticks_end - ticks_start;
        double seconds = ticks_delta / CLOCKS_PER_SEC;

        (void) seconds;
        // printf("                            %10.2lfs\r%.60s:\n", seconds, algorithm_names[alg_i]);
    }

    return EXIT_SUCCESS;
}

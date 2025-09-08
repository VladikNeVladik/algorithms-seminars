// Copyright 2024 Vladislav Aleinik

#include <stdio.h>
#include <math.h>

//============================//
// Оценки за домашние задания //
//============================//

#define NUM_TASKS 7U

// Значения hwi[j] должны лежать на отрезке [0; 1.0].
//                     |             Задача №№            |
// ДЗ #1:              | 01 | 02 | 03 | 04 | 05 | 06 | 07 |
float hw1[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// ДЗ #2:              |    |    |    |    |    |    |    |
float hw2[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// ДЗ #3:              |    |    |    |    |    |    |    |
float hw3[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// ДЗ #4:              |    |    |    |    |    |    |    |
float hw4[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// ДЗ #5:              |    |    |    |    |    |    |    |
float hw5[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// ДЗ #6:              |    |    |    |    |    |    |    |
float hw6[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

// Штраф за плагиат:
// Значения hwi[j] должны быть не более, чем сумма оценок за соответствующее ДЗ.
float penalties[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

//====================//
// Контрольные работы //
//====================//

// "test" - это "Контрольная работа".
// Значение переменных test лежат на отрезке [0; 10].
float test1 = 1, test2 = 1, test3 = 1;

//=========//
// Экзамен //
//=========//

// Экзамен - число на отрезке [0; 10].
float exam = 10;

//==================================//
// Вычисление результата за семестр //
//==================================//

int main(void)
{
    // Суммы баллов за домашние задания.
    float hw1_sum = 0.0, hw2_sum = 0.0, hw3_sum = 0.0;
    float hw4_sum = 0.0, hw5_sum = 0.0, hw6_sum = 0.0;

    // Количества сделанных задачек (по каждому ДЗ).
    int hw1_num_tasks = 0, hw2_num_tasks = 0, hw3_num_tasks = 0;
    int hw4_num_tasks = 0, hw5_num_tasks = 0, hw6_num_tasks = 0;
    for (int i = 0; i < NUM_TASKS; ++i)
    {
        hw1_num_tasks += (hw1[i] > 0.01);
        hw2_num_tasks += (hw2[i] > 0.01);
        hw3_num_tasks += (hw3[i] > 0.01);
        hw4_num_tasks += (hw4[i] > 0.01);
        hw5_num_tasks += (hw5[i] > 0.01);
        hw6_num_tasks += (hw6[i] > 0.01);

        hw1_sum += hw1[i];
        hw2_sum += hw2[i];
        hw3_sum += hw3[i];
        hw4_sum += hw4[i];
        hw5_sum += hw5[i];
        hw6_sum += hw6[i];
    }

    hw1_sum = hw1_sum / NUM_TASKS - penalties[0];
    hw2_sum = hw2_sum / NUM_TASKS - penalties[1];
    hw3_sum = hw3_sum / NUM_TASKS - penalties[2];
    hw4_sum = hw4_sum / NUM_TASKS - penalties[3];
    hw5_sum = hw5_sum / NUM_TASKS - penalties[4];
    hw6_sum = hw6_sum / NUM_TASKS - penalties[5];

    // Производим "срез" за невыполненное домашнее задание.
    float hw_total = 0.0;
    if (hw1_num_tasks >= 3 && hw2_num_tasks >= 3 && hw3_num_tasks >= 3 &&
        hw4_num_tasks >= 3 && hw5_num_tasks >= 3 && hw6_num_tasks >= 3)
    {
        hw_total = 0.5 * (hw1_sum + hw2_sum + hw3_sum +
                          hw4_sum + hw5_sum + hw6_sum);
    }

    // Контрольные работы
    float test_total = 1.0 * (test1 + test2 + test3);

    // Compute semester total:
    float exam_total = 0.4 * exam;

    float sem_total = 0.0;
    // Используем "<" вместо "<=" для сравнения чисел с плавающей точкой.
    if (exam < 3.0)
    {
        sem_total = exam_total;
    }
    else
    {
        sem_total = hw_total + test_total + exam_total;
    }

    float result = roundf(sem_total);

    printf("Your grade is: %.02f\n", result);

    return 0;
}

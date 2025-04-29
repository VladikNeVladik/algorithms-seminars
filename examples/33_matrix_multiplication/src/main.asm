bits 32

extern io_print_hex, io_print_char, io_print_string, io_newline
extern compare_matrix_multiplication_time

; Constants:
MATRIX_SIZE     equ    32 ;   4 KB
; MATRIX_SIZE     equ    64 ;  16 KB
; MATRIX_SIZE     equ   128 ;  64 KB
; MATRIX_SIZE     equ   256 ; 256 KB  0,03 с
; MATRIX_SIZE     equ   512 ;   1 MB  0,22 с
; MATRIX_SIZE     equ  1024 ;   4 MB  2,44 с
; MATRIX_SIZE     equ  2048 ;  16 MB 64,07 с
; MATRIX_SIZE     equ  4096 ;  64 MB ????? с

; Размещаем матрицы в секции .bss (секция для неинициализированных данных)
section .bss
; Кодирование двумерного индекса одномерным:
; matrix[y][x] = matrix + 4 * (MATRIX_SIZE * y + x)
mat_A       resd MATRIX_SIZE * MATRIX_SIZE
mat_B       resd MATRIX_SIZE * MATRIX_SIZE
mat_C       resd MATRIX_SIZE * MATRIX_SIZE
eax_tmp     resd 1
ebx_tmp     resd 1

section .text
global main
main:
    ; Дополнительно вызываем функцию для сравнения алгоритмов перемножения матриц.
    ; Выравнивание стека = "нам повезёт!".
    call compare_matrix_multiplication_time

    ;==========================;
    ; Инициализируем матрицу A ;
    ;==========================;

    ; Назначение регистров:
    ; EAX = y
    ; EBX = x
    ; ECX = &matrix[y][x]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, mat_A
.mat_A_init_out_loop:
    cmp     eax, MATRIX_SIZE
    jae     .mat_A_init_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .mat_A_init_in_loop:
        cmp     ebx, MATRIX_SIZE
        jae     .mat_A_init_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     esi, 0x1
        mov     edi, 0x0

        cmp     eax, ebx
        cmovbe  edx, esi
        cmova   edx, edi

        mov     dword [ecx], edx

        ; Обновляем счётчики циклов.
        add     ebx, 1
        add     ecx, 4
        jmp     .mat_A_init_in_loop
    .mat_A_init_in_loop_exit:

    ; Обновляем счётчики циклов.
    add     eax, 1
    jmp     .mat_A_init_out_loop
.mat_A_init_out_loop_exit:

    ;==========================;
    ; Инициализируем матрицу B ;
    ;==========================;

    ; Назначение регистров:
    ; EAX = x
    ; EBX = y
    ; ECX = matrix[y][x]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, mat_B
.mat_B_init_out_loop:
    cmp     eax, MATRIX_SIZE
    jae     .mat_B_init_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .mat_B_init_in_loop:
        cmp     ebx, MATRIX_SIZE
        jae     .mat_B_init_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     esi, 0x1
        mov     edi, 0x0

        cmp     eax, ebx
        cmovbe  edx, esi
        cmova   edx, edi

        mov     dword [ecx], edx

        ; Обновляем счётчики циклов.
        add     ebx, 1
        add     ecx, 4
        jmp     .mat_B_init_in_loop
    .mat_B_init_in_loop_exit:

    ; Обновляем счётчики циклов.
    add     eax, 1
    jmp     .mat_B_init_out_loop
.mat_B_init_out_loop_exit:

    ;=======================;
    ; Умножение двух матриц ;
    ;=======================;

    ; Назначение регистров:
    ; EAX = c_y = a_y
    ; EBX = c_x = b_x = mul accum
    ; ECX = &mat_C[c_y][c_x]
    ; EDX = a_x = b_y
    ; ESI = &mat_A[a_y][a_x]
    ; EDI = &mat_B[b_y][b_x]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, mat_C
.mat_C_calc_out_loop:
    cmp     eax, MATRIX_SIZE
    jae     .mat_C_calc_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .mat_C_calc_in_loop:
        cmp     ebx, MATRIX_SIZE
        jae     .mat_C_calc_in_loop_exit

        ; Сохраняем EAX, EBX во временные ячейки памяти.
        mov     dword [eax_tmp], eax
        mov     dword [ebx_tmp], ebx

        ; Загружаем в EDI адрес в матрице B.
        lea     edi, dword [mat_B + 4 * ebx]

        ; Аналогичная загрузка адреса в матрице A с помощью LEA не работает.
        ; Значение 4 * MATRIX_SIZE слишком велико.
        ; lea   esi, dword [mat_A + 4 * MATRIX_SIZE * ebx]
        imul    esi, eax, 4 * MATRIX_SIZE
        add     esi, mat_A

        ; Инициализируем счётчики третьего внутреннего цикла.
        ; EAX = результат умножения
        ; EBX = регистр для хранения частичной суммы произведений.
        ; EDX = index
        mov     ebx, 0
        mov     edx, 0

        .mat_C_mult_loop:
            cmp     edx, MATRIX_SIZE
            jae     .mat_C_mult_loop_exit

            ; Обновляем частичную сумму.
            mov     eax, dword [esi]
            imul    eax, dword [edi]
            add     ebx, eax

            ; Обновляем индексы.
            add     edx, 1
            add     esi, 4
            add     edi, 4 * MATRIX_SIZE
            ; ^^^^^^^^^^^^^^^^^^^^^^^^^^
            ; Это изменение индекса приводит к выходу за границы кэш-линии.
            ; Это неэффективно, и потому рекомендуется транспонировать матрицу перед умножением.
            jmp     .mat_C_mult_loop
        .mat_C_mult_loop_exit:

        ; Сохраняем результат mat_C[c_y][c_x] в матрицу C.
        mov     dword [ecx], ebx

        ; Восстанавливаем EAX, EBX из временных переменных в памяти.
        mov     eax, dword [eax_tmp]
        mov     ebx, dword [ebx_tmp]

        ; Обновляем счётчики цикла.
        add     ebx, 1
        add     ecx, 4
        jmp     .mat_C_calc_in_loop
    .mat_C_calc_in_loop_exit:

    ; Обновляем счётчики цикла.
    add     eax, 1
    jmp     .mat_C_calc_out_loop
.mat_C_calc_out_loop_exit:

    ;==============;
    ; Ранний выход ;
    ;==============;

    ; Будем использовать для измерения времени.
    ; xor     eax, eax
    ; ret

    ;=============================;
    ; Выводим результат в консоль ;
    ;=============================;

    ; Назначение регистров:
    ; ESI = x
    ; EDI = y
    ; EBX = &matrix[y][x]

    ; Инициализируем счётчики внешнего цикла.
    mov     esi, 0
    mov     ebx, mat_C
.mat_C_print_out_loop:
    cmp     esi, MATRIX_SIZE
    jae     .mat_C_print_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     edi, 0
    .mat_C_print_in_loop:
        cmp     edi, MATRIX_SIZE
        jae     .mat_C_print_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     eax, dword [ebx]
        call    io_print_hex

        ; Печатаем пробел.
        mov     eax, ' '
        call    io_print_char

        ; Обновляем счётчики цикла.
        add     edi, 1
        add     ebx, 4
        jmp     .mat_C_print_in_loop
    .mat_C_print_in_loop_exit:

    ; Печатаем перенос строки.
    call    io_newline

    ; Обновляем счётчики цикла.
    add     esi, 1
    jmp     .mat_C_print_out_loop
.mat_C_print_out_loop_exit:

    xor     eax, eax
    ret

bits 32

extern io_print_hex, io_print_char, io_print_string, io_newline

;====================================;
; Константы и функциональные макросы ;
;====================================;

; Напоминание - структура стевового фрейма согласно System V ABI для архитектуры i386.
; [ebp + 16]            ...
; [ebp + 12] memory argument fourbyte 1
; [ebp +  8] memory argument fourbyte 0
; <После записи аргументов функции на стек значение регистра ESP выровнено на 16 байт>
; [ebp +  4] адрес возврата
; [ebp +  0] Предыдущее значение регистра EBP
; [ebp -  4] varN
;     ...
; [esp +  4] var1
; [esp +  0] var0

; Выравниваем стек на 16 байт после записи %1 байт на вершину стека
; Псевдоинструкция "enter X, 0" эквивалентна:
; push  ebp
; mov   ebp, esp
; sub   esp, X
%macro FUNCTION_PROLOGUE 1.nolist
    enter   %1, 0
%endmacro

; Псевдоинструкция "leave" эквивалентна:
; mov   esp, ebp
; pop   ebp
%macro FUNCTION_EPILOGUE 0.nolist
    leave
%endmacro

; Делаем так, чтобы стек был выровнен на 16 байт после записи на вершину стека %1 байт.
; Этот макрос не создаёт дополнительный стековый фрейм.
%macro ALIGN_STACK 1.nolist
    sub     esp, %1
    and     esp, 0xfffffff0
    add     esp, %1
%endmacro

; Возвращаем стек к исходному состоянию.
%macro UNALIGN_STACK 1.nolist
    add     esp, %1
%endmacro

; Константы:
MATRIX_SIZE     equ    16
; MATRIX_SIZE     equ    1024
; MATRIX_SIZE     equ    2048

;=======================;
; Инициализация матрицы ;
;=======================;

global matrix_init
%define size_y          dword [ebp + 16]
%define size_x          dword [ebp + 12]
%define matrix_base     dword [ebp +  8]
%define tmp_ebx         dword [ebp -  4]
%define tmp_edi         dword [ebp -  8]
%define tmp_esi         dword [ebp - 12]
matrix_init:
    ; Инициализируем стековый фремй
    FUNCTION_PROLOGUE 12
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Назначение регистров:
    ; EAX = y
    ; EBX = x
    ; ECX = &matrix[y][x]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, matrix_base
.matrix_init_out_loop:
    cmp     eax, size_y
    jae     .matrix_init_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .matrix_init_in_loop:
        cmp     ebx, size_x
        jae     .matrix_init_in_loop_exit

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
        jmp     .matrix_init_in_loop
    .matrix_init_in_loop_exit:

    ; Обновляем счётчики циклов.
    add     eax, 1
    jmp     .matrix_init_out_loop
.matrix_init_out_loop_exit:

    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi

    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef size_y
%undef size_x
%undef matrix_base
%undef tmp_ebx
%undef tmp_edi
%undef tmp_esi

;============================;
; Печать содержимого матрицы ;
;============================;

global matrix_print
%define size_y          dword [ebp + 16]
%define size_x          dword [ebp + 12]
%define matrix_base     dword [ebp +  8]
%define tmp_ebx         dword [ebp -  4]
%define tmp_edi         dword [ebp -  8]
%define tmp_esi         dword [ebp - 12]
matrix_print:
    ; Инициализируем стековый фремй
    FUNCTION_PROLOGUE 12
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Назначение регистров:
    ; ESI = x
    ; EDI = y
    ; EBX = &matrix[y][x]

    ; Инициализируем счётчики внешнего цикла.
    mov     esi, 0
    mov     ebx, matrix_base
.matrix_print_out_loop:
    cmp     esi, size_y
    jae     .matrix_print_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     edi, 0
    .matrix_print_in_loop:
        cmp     edi, size_x
        jae     .matrix_print_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     eax, dword [ebx]
        call    io_print_hex

        ; Выводим пробельный символ.
        mov     eax, ' '
        call    io_print_char

        ; Обновляем счётчики циклов.
        add     edi, 1
        add     ebx, 4
        jmp     .matrix_print_in_loop
    .matrix_print_in_loop_exit:

    ; Выводим символ переноса строки.
    call    io_newline

    ; Обновляем счётчики циклов.
    add     esi, 1
    jmp     .matrix_print_out_loop
.matrix_print_out_loop_exit:

    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi

    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef size_y
%undef size_x
%undef matrix_base
%undef tmp_ebx
%undef tmp_edi
%undef tmp_esi

;=============================;
; Простое перемножение матриц ;
;=============================;

; Список аргументов без дублирований.
; base_C
; size_B_x = size_C_x
; base_B
; size_A_y = size_C_y
; size_A_x = size_B_y
; base_A

global matrix_multiply
; Аргументы функции.
%define size_C_y        dword [ebp + 16]
%define size_C_x        dword [ebp + 24]
%define base_C          dword [ebp + 28]
%define size_B_y        dword [ebp + 12]
%define size_B_x        dword [ebp + 24]
%define base_B          dword [ebp + 20]
%define size_A_y        dword [ebp + 16]
%define size_A_x        dword [ebp + 12]
%define base_A          dword [ebp +  8]
; Автоматические переменные функции.
%define tmp_ebx         dword [ebp -  4]
%define tmp_edi         dword [ebp -  8]
%define tmp_esi         dword [ebp - 12]
%define idx_C_y         dword [ebp - 16]
%define idx_C_x         dword [ebp - 20]
matrix_multiply:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 20
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Назначение регистров:
    ; EAX = c_y = a_y
    ; EBX = c_x = b_x = mul accum
    ; ECX = &mat_C[c_y][c_x]
    ; EDX = a_x = b_y
    ; ESI = &mat_A[a_y][a_x]
    ; EDI = &mat_B[b_y][b_x]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, base_C
.matrix_multiply_out_loop:
    cmp     eax, size_C_y
    jae     .matrix_multiply_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .matrix_multiply_in_loop:
        cmp     ebx, size_A_x
        jae     .matrix_multiply_in_loop_exit

        ; Сохраняем значения регистров EAX, EBX.
        mov     idx_C_y, eax
        mov     idx_C_x, ebx

        ; Инициализируем счётчики третьего вложенного цикла.
        ; EDI = base_B + 4 * EBX
        imul    edi, ebx, 4
        add     edi, base_B

        ; ESI = base_A + 4 * size_A_x * EAX:
        imul    esi, eax, 4
        imul    esi, size_A_x
        add     esi, base_A

        ; Инициализируем счётчики третьего внутреннего цикла.
        ; EAX = результат умножения
        ; EBX = регистр для хранения частичной суммы произведений
        ; EDX = индекс итерирования
        mov     ebx, 0
        mov     edx, 0

        .matrix_multiply_mult_loop:
            cmp     edx, size_C_x
            jae     .matrix_multiply_mult_loop_exit

            ; Обновляем частичную сумму.
            mov     eax, dword [esi]
            imul    eax, dword [edi]
            add     ebx, eax

            ; Обновляем индексы.
            add     edx, 1
            add     esi, 4
            imul    eax, size_A_x, 4
            add     edi, eax
            jmp     .matrix_multiply_mult_loop
        .matrix_multiply_mult_loop_exit:

        ; Сохраняем mat_C[c_y][c_x] в память матрицы C.
        mov     dword [ecx], ebx

        ; Восстанавливаем значение регисров EAX, EBX из памяти.
        mov     eax, idx_C_y
        mov     ebx, idx_C_x

        ; Обновляем счётчики цикла.
        add     ebx, 1
        add     ecx, 4
        jmp     .matrix_multiply_in_loop
    .matrix_multiply_in_loop_exit:

    ; Обновляем счётчики цикла.
    add     eax, 1
    jmp     .matrix_multiply_out_loop
.matrix_multiply_out_loop_exit:

    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi

    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef size_C_y
%undef size_C_x
%undef base_C
%undef size_B_y
%undef size_B_x
%undef base_B
%undef size_A_y
%undef size_A_x
%undef base_A
; Автоматические переменные функции.
%undef tmp_ebx
%undef tmp_edi
%undef tmp_esi
%undef idx_C_y
%undef idx_C_x

;=========================================;
; Перемножение матриц с транспонированием ;
;=========================================;

; Список аргументов без дублирований.
; base_C
; size_B_x = size_C_x
; base_B
; size_A_y = size_C_y
; size_A_x = size_B_y
; base_A

section .text
global matrix_multiply_tr
; Аргументы функции.
%define size_C_y        dword [ebp + 16]
%define size_C_x        dword [ebp + 24]
%define base_C          dword [ebp + 28]
%define size_B_y        dword [ebp + 12]
%define size_B_x        dword [ebp + 24]
%define base_B          dword [ebp + 20]
%define size_A_y        dword [ebp + 16]
%define size_A_x        dword [ebp + 12]
%define base_A          dword [ebp +  8]
; Автоматические переменные функции.
%define tmp_ebx         dword [ebp -  4]
%define tmp_edi         dword [ebp -  8]
%define tmp_esi         dword [ebp - 12]
%define idx_C_y         dword [ebp - 16]
%define idx_C_x         dword [ebp - 20]
matrix_multiply_tr:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 20
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ;----------------------------------;
    ; Транспонируем элементы матрицы B ;
    ;----------------------------------;

    ; Назначение регистров:
    ; EAX = y
    ; EBX = x
    ; ECX = &mat_B[y][x]
    ; EDX = &mat_B[x][y]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
.matrix_transpose_out_loop:
    cmp     eax, size_B_y
    jae     .matrix_transpose_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0

    mov     ecx, base_B
    imul    esi, eax, 4
    imul    esi, size_B_x
    add     ecx, esi

    mov     edx, base_B
    imul    esi, eax, 4
    add     edx, esi
    .matrix_transpose_in_loop:
        cmp     ebx, eax
        jae     .matrix_transpose_in_loop_exit

        mov     esi, dword [ecx]
        mov     edi, dword [edx]

        mov     dword [ecx], edi
        mov     dword [edx], esi

        ; Обновляем счётчики цикла.
        add     ebx, 1
        add     ecx, 4
        imul    esi, size_B_y, 4
        add     edx, esi
        jmp     .matrix_transpose_in_loop
    .matrix_transpose_in_loop_exit:

    ; Обновляем счётчики цикла.
    add     eax, 1
    jmp     .matrix_transpose_out_loop
.matrix_transpose_out_loop_exit:

    ; Меняем местами размеры матрицы B.
    mov esi, size_B_x
    mov edi, size_B_y

    mov size_B_x, edi
    mov size_B_y, esi

    ;-------------------------------------------------;
    ; Производим оптимизированное произведение матриц ;
    ;-------------------------------------------------;

    ; Назначение регистров:
    ; EAX = c_y = a_y
    ; EBX = c_x = b_x = mul accum
    ; ECX = &mat_C[c_y][c_x]
    ; EDX = a_x = b_y
    ; ESI = &mat_A[a_y][a_x]
    ; EDI = &mat_B[b_x][b_y]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, base_C
.matrix_multiply_tr_out_loop:
    cmp     eax, size_C_y
    jae     .matrix_multiply_tr_out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .matrix_multiply_tr_in_loop:
        cmp     ebx, size_C_x
        jae     .matrix_multiply_tr_in_loop_exit

        ; Сохраняем значения регистров EAX, EBX.
        mov     idx_C_y, eax
        mov     idx_C_x, ebx

        ; Инициализируем текущии позиции в матрицах.
        ; EDI = base_B + 4 * size_B_x * EBX
        imul    edi, ebx, 4
        imul    edi, size_B_x
        add     edi, base_B

        ; ESI = base_A + 4 * size_A_x * EAX:
        imul    esi, eax, 4
        imul    esi, size_A_x
        add     esi, base_A

        ; Инициализируем счётчики третьего внутреннего цикла.
        ; EAX = результат умножения
        ; EBX = регистр для хранения частичной суммы произведений
        ; EDX = индекс итерирования
        mov     ebx, 0
        mov     edx, 0

        .matrix_multiply_tr_mult_loop:
            cmp     edx, size_A_x
            jae     .matrix_multiply_tr_mult_loop_exit

            ; Обновляем частичную сумму.
            mov     eax, dword [esi]
            imul    eax, dword [edi]
            add     ebx, eax

            ; Обновляем счётчики цикла.
            add     edx, 1
            add     esi, 4
            add     edi, 4
            jmp     .matrix_multiply_tr_mult_loop
        .matrix_multiply_tr_mult_loop_exit:

        ; Записываем элемент mat_C[c_y][c_x] матрицы C.
        mov     dword [ecx], ebx

        ; Восстанавливаем EAX, EBX из памяти.
        mov     eax, idx_C_y
        mov     ebx, idx_C_x

        ; Обновляем счётчики цикла.
        add     ebx, 1
        add     ecx, 4
        jmp     .matrix_multiply_tr_in_loop
    .matrix_multiply_tr_in_loop_exit:

    ; Обновляем счётчики цикла.
    add     eax, 1
    jmp     .matrix_multiply_tr_out_loop
.matrix_multiply_tr_out_loop_exit:

    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi

    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef size_C_y
%undef size_C_x
%undef base_C
%undef size_B_y
%undef size_B_x
%undef base_B
%undef size_A_y
%undef size_A_x
%undef base_A
; Автоматические переменные функции.
%undef tmp_ebx
%undef tmp_edi
%undef tmp_esi
%undef idx_C_y
%undef idx_C_x

;===============;
; Main function ;
;===============;

; Размещаем матрицы в секции неинициаизированных данных.
section .bss
; Доступ к элементу матрицы осуществляется так: matrix[y][x] = matrix + 4 * (MATRIX_SIZE * y + x).
mat_A:          resd MATRIX_SIZE * MATRIX_SIZE
mat_B:          resd MATRIX_SIZE * MATRIX_SIZE
mat_C:          resd MATRIX_SIZE * MATRIX_SIZE

section .text

global main
%define tmp_ebx dword [ebp -  4]
%define tmp_edi dword [ebp -  8]
%define tmp_esi dword [ebp - 12]
main:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 12
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Инициализируем матрицу A.
    ALIGN_STACK 12
    push MATRIX_SIZE ; size_y
    push MATRIX_SIZE ; size_x
    push mat_A       ; matrix_base
    call matrix_init
    UNALIGN_STACK 12

    ; Инициализируем матрицу B.
    ALIGN_STACK 12
    push MATRIX_SIZE ; size_y
    push MATRIX_SIZE ; size_x
    push mat_B       ; matrix_base
    call matrix_init
    UNALIGN_STACK 12

    ; Перемножение матриц.
    ALIGN_STACK 24
    push mat_C
    push MATRIX_SIZE
    push mat_B
    push MATRIX_SIZE
    push MATRIX_SIZE
    push mat_A
    call matrix_multiply_tr
    UNALIGN_STACK 24

    ; Выводим содежимое матрицы.
    ALIGN_STACK 12
    push MATRIX_SIZE ; size_y
    push MATRIX_SIZE ; size_x
    push mat_C       ; matrix_base
    call matrix_print
    UNALIGN_STACK 12

    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi

    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    xor     eax, eax
    ret

; Copyright 2025 Vladislav Aleinik
bits 32

;=====================;
; Внешние зависимости ;
;=====================;

; stdio.h
extern printf, scanf
; stdlib.h
extern calloc, free

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

;=======================;
; Инициализация матрицы ;
;=======================;

section .text
global matrix_init
%define     size_y      dword [ebp + 16]
%define     size_x      dword [ebp + 12]
%define     matrix_base dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
matrix_init:
    ; Создаём стековый фрейм.
    FUNCTION_PROLOGUE 4
    ; Сохраняем значение регистра EBX.
    mov     tmp_ebx, ebx
    ; Не сохраняем управляющее слово FPU, т.к. оно остаётся неизменным.
    ; Не сохраняем статусное слово FPU и регистры ST(i)

    ; Назначение регистров:
    ; EAX = y
    ; EBX = x
    ; ECX = matrix[y][x]

    ; Подготавливаем значения на стеке FPU для условных переходов.
    fldz    ; ST(2)
    fld1    ; ST(1)
    fldz    ; ST(0)

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, matrix_base
.out_loop:
    cmp     eax, size_y
    jae     .out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .in_loop:
        cmp     ebx, size_x
        jae     .in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        cmp     eax, ebx

        fcmovbe     st0, st1
        fcmovnbe    st0, st2
        fst         dword [ecx]

        ; Обновляем счётчики цикла.
        add     ebx, 1
        add     ecx, 4
        jmp     .in_loop
    .in_loop_exit:

    ; Обновляем счётчики цикла.
    add     eax, 1
    jmp     .out_loop
.out_loop_exit:

.exit:
    ; Восстанавливаем регистр EBX.
    mov     ebx, tmp_ebx
    ; Восстанавливаем предыдущий стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся в вызывающую функцию.
    ret

; Избавляемся от макросов в целях безопасности.
%undef      size_y
%undef      size_x
%undef      matrix_base
%undef      tmp_ebx

;====================;
; Распечатка матрицы ;
;====================;

section .text
global matrix_print
%define     size_y      dword [ebp + 16]
%define     size_x      dword [ebp + 12]
%define     matrix_base dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
matrix_print:
    ; Создаём стековый фрейм.
    FUNCTION_PROLOGUE 12
    ; Сохраняем значение регистров.
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi
    ; Не сохраняем управляющее слово FPU, т.к. оно остаётся неизменным.
    ; Не сохраняем статусное слово FPU и регистры ST(i)

    ; Index variables:
    ; esi = x
    ; edi = y
    ; ebx = matrix[y][x]

    ; Инициализируем счётчики внешнего цикла.
    mov     esi, 0
    mov     ebx, matrix_base
.out_loop:
    cmp     esi, size_y
    jae     .out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     edi, 0
    .in_loop:
        cmp     edi, size_x
        jae     .in_loop_exit

        fld     dword [ebx]

        ; Печатаем matrix[y][x];
        ALIGN_STACK 12
        sub     esp, 8
        fstp    qword [esp]
        push    str_format_matrix_elem
        call    printf
        UNALIGN_STACK 12

        ; Обновляем счётчики цикла.
        add     edi, 1
        add     ebx, 4
        jmp     .in_loop
    .in_loop_exit:

    ; Выводим перенос строки.
    ALIGN_STACK 4
    push    str_format_newline
    call    printf
    UNALIGN_STACK 4

    ; Обновляем счётчики цикла.
    add     esi, 1
    jmp     .out_loop
.out_loop_exit:

.exit:
    ; Восстанавливаем регистры.
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Восстанавливаем предыдущий стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся в вызывающую функцию.
    ret

; Избавляемся от макросов в целях безопасности.
%undef      size_y
%undef      size_x
%undef      matrix_base
%undef      tmp_ebx
%undef      tmp_edi
%undef      tmp_esi

section .rodata
str_format_matrix_elem:     db "%5.1f ", 0
str_format_newline:         db 0xA, 0

;==================;
; Умножение матриц ;
;==================;

; Фактический список параметров:
; base_C
; size_B_x = size_C_x
; base_B
; size_A_y = size_C_y
; size_A_x = size_B_y
; base_A

section .text
global matrix_multiply
; Аргументы функции:
%define size_C_y        dword [ebp + 16]
%define size_C_x        dword [ebp + 24]
%define base_C          dword [ebp + 28]
%define size_B_y        dword [ebp + 12]
%define size_B_x        dword [ebp + 24]
%define base_B          dword [ebp + 20]
%define size_A_y        dword [ebp + 16]
%define size_A_x        dword [ebp + 12]
%define base_A          dword [ebp +  8]
; Автоматические переменные функции:
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
%define     idx_C_y     dword [ebp - 16]
%define     idx_C_x     dword [ebp - 20]
%define     fpu_ctrl    word  [ebp - 22]
matrix_multiply:
    ; Создаём стековый фрейм.
    FUNCTION_PROLOGUE 22
    ; Сохраняем значения регистров.
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi
    ; Сохраняем слово управления FPU на стек.
    ; Сбрасываем слово статуса FPU.
    ; Это необходимо, чтобы ловить QNaN при переполнении стека FPU.
    fstcw   fpu_ctrl
    finit

    ; Назначение регистров:
    ; EAX = c_y = a_y
    ; EBX = c_x = b_x
    ; ECX = &mat_C[c_y][c_x]
    ; EDX = a_x = b_y
    ; ESI = &mat_A[a_y][a_x]
    ; EDI = &mat_B[b_y][b_x]

    ; Инициализируем счётчики внешнего цикла.
    mov     eax, 0
    mov     ecx, base_C
.out_loop:
    cmp     eax, size_C_y
    jae     .out_loop_exit

    ; Инициализируем счётчики внутреннего цикла.
    mov     ebx, 0
    .in_loop:
        cmp     ebx, size_A_x
        jae     .in_loop_exit

        ; Сохраняем EAX, EBX во временную память.
        mov     idx_C_y, eax
        mov     idx_C_x, ebx

        ; Инициализируем счётчики цикла.
        ; EDI = base_B + 4 * EBX
        imul    edi, ebx, 4
        add     edi, base_B

        ; ESI = base_A + 4 * size_A_x * EAX:
        imul    esi, eax, 4
        imul    esi, size_A_x
        add     esi, base_A

        ; Инициализируем аккумулятор цикла.
        ; ST(0) = текущая сумма
        ; EDX   = индекс
        fldz

        mov     ebx, 0
        mov     edx, 0

        .mult_loop:
            cmp     edx, size_C_x
            jae     .mult_loop_exit

            ; Обновляем текущую сумму.
            fld     dword [esi]
            fld     dword [edi]
            fmulp

            faddp

            ; Обновляем счётчики цикла.
            add     edx, 1
            add     esi, 4
            imul    eax, size_A_x, 4
            add     edi, eax
            jmp     .mult_loop
        .mult_loop_exit:

        ; Сахраняем mat_C[c_y][c_x] в матрицу C.
        fstp    dword [ecx]

        ; Восстанавливаем EAX, EBX из памяти.
        mov     eax, idx_C_y
        mov     ebx, idx_C_x

        ; Обновляем счётчики цикла.
        add     ebx, 1
        add     ecx, 4
        jmp     .in_loop
    .in_loop_exit:

    ; Обновляем счётчики цикла.
    add     eax, 1
    jmp     .out_loop
.out_loop_exit:

.exit:
    ; Восстанавливаем слово управления FPU.
    fldcw   fpu_ctrl

    ; Восстанавливаем регистры FPU.
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся в вызывающую функцию.
    ret

; Избавляемся от макросов в целях безопасности.
%undef      size_C_y
%undef      size_C_x
%undef      base_C
%undef      size_B_y
%undef      size_B_x
%undef      base_B
%undef      size_A_y
%undef      size_A_x
%undef      base_A
%undef      tmp_ebx
%undef      tmp_edi
%undef      tmp_esi
%undef      idx_C_y
%undef      idx_C_x

;==============;
; Функция main ;
;==============;

; Размер матрицы.
MATRIX_SIZE     equ    8

section .text
global main
%define     mat_A   dword [ebp -  4]
%define     mat_B   dword [ebp -  8]
%define     mat_C   dword [ebp - 12]
main:
    ; Аллоцируем стековый фрейм.
    FUNCTION_PROLOGUE 12

    ;===========================================;
    ; Аллоцируем динамическую память для матриц ;
    ;===========================================;

    ; Аллоцируем матрицу A.
    ALIGN_STACK 8
    push    4                           ; size
    push    MATRIX_SIZE * MATRIX_SIZE   ; nmemb
    call    calloc
    UNALIGN_STACK 8

    ; ПРЕДПОЛОЖЕНИЕ: mat_A != NULL
    mov     mat_A, eax

    ; Аллоцируем матрицу B.
    ALIGN_STACK 8
    push    4                           ; size
    push    MATRIX_SIZE * MATRIX_SIZE   ; nmemb
    call    calloc
    UNALIGN_STACK 8

    ; ПРЕДПОЛОЖЕНИЕ: mat_B != NULL
    mov     mat_B, eax

    ; Аллоцируем матрицу C.
    ALIGN_STACK 8
    push    4                           ; size
    push    MATRIX_SIZE * MATRIX_SIZE   ; nmemb
    call    calloc
    UNALIGN_STACK 8

    ; ПРЕДПОЛОЖЕНИЕ: mat_C != NULL
    mov     mat_C, eax

    ; Заполняем матрицы значениями.
    ALIGN_STACK 12
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    mov     eax, mat_A
    push    eax
    call    matrix_init
    UNALIGN_STACK 12

    ALIGN_STACK 12
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    mov     eax, mat_B
    push    eax
    call    matrix_init
    UNALIGN_STACK 12

    ; Умножаем матрицы.
    ALIGN_STACK 24
    push    mat_C
    push    MATRIX_SIZE
    push    mat_B
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    push    mat_A
    call    matrix_multiply
    UNALIGN_STACK 24

    ; Выводим результат умножения.
    ALIGN_STACK 12
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    mov     eax, mat_C
    push    eax
    call    matrix_print
    UNALIGN_STACK 12

    ; Освобождаем память.
    ALIGN_STACK 4
    mov     eax, mat_A
    push    eax
    call    free
    UNALIGN_STACK 4

    ALIGN_STACK 4
    mov     eax, mat_B
    push    eax
    call    free
    UNALIGN_STACK 4

    ALIGN_STACK 4
    mov     eax, mat_C
    push    eax
    call    free
    UNALIGN_STACK 4

    ; return 0
    xor     eax, eax
.exit:
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ret

%undef     mat_A
%undef     mat_B
%undef     mat_C

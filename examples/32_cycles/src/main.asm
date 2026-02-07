; Copyright 2025 Vladislav Aleinik
bits 32

extern io_print_udec, io_print_hex, io_get_udec, io_print_string, io_newline

; Макроконстанты (подставляются при трансляции ассемблерного кода).
MAX_ARR_SIZE  equ     10
EXIT_FAILURE  equ     1

section .text
global main
main:
    mov ebp, esp; for correct debugging
    ; Указатель на стековый фрейм необходим для корректной работы отладчика. 

    ; Вводим размер массива.
    mov     eax, str_enter_size
    call    io_print_string

    call    io_get_udec
    mov     dword [size], eax

    ; Сравниваем размер массива с максимальным заложенным в программу.
    cmp     eax, MAX_ARR_SIZE
    jbe     .verify_array_size_ok

    ; Выводим сообщение об ошибке и выходим.
    mov     eax, str_array_size_too_big
    call    io_print_string

    mov     eax, EXIT_FAILURE
    ret

.verify_array_size_ok:

    ;===================;
    ; Цикл ввода данных ;
    ;===================;

    ; Инициализируем индекс в массиве.
    ; [ECX] := индекс в массиве
    mov     ecx, 0

.input_loop:
    cmp     ecx, dword [size]
    jae     .input_loop_exit

    ; Сохраняем регистр ecx (он может быть "перетёрт" во время вызова функции).
    mov     dword [index], ecx
    call    io_get_udec
    mov     ecx, dword [index]

    ; Сохраняем введённые данные в память.
    mov     dword [arr + 4*ecx], eax

    ; Обновляем значение счётчика.
    inc     ecx

    jmp     .input_loop
.input_loop_exit:

    ;=========================;
    ; Ищем максимальное число ;
    ;=========================;

    ; Инициализируем счётчики цикла.
    ; [ECX] := Индекс в массиве.
    ; [EBX] := Текущее максимальное число.
    mov     ebx, 0
    mov     ecx, dword [size]

.max_search_loop:
    ; [EAX] := текущий претендент на звание максимума в массиве.
    mov     eax, dword [arr + 4*ecx - 4]

    cmp     ebx, eax
    cmovb   ebx, eax

    loop .max_search_loop

    ; Выводим максимальное число в массиве.
    mov eax, str_max_value_is
    call io_print_string

    mov eax, ebx
    call io_print_udec
    call io_newline

    ;==================================================;
    ; Конвертируем число в 16-ричную систему счисления ;
    ;==================================================;

    ; [EAX] := Число для преобразования в 16-ричную систему.
    ; [ECX] := Кол-во 16-ричных разрядов, оставшихся для преобразования.
    mov eax, ebx
    mov ecx, 8

.loop_convert_hex2char:
    ; [EDX:EAX] := Число для преобразования в 16-ричную систему.
    xor edx, edx
    mov ebx, 16
    idiv ebx
    ; EDX = EDX:EAX % EBX
    ; EAX = EDX:EAX / EBX

    ; Извлекаем символ, соответствующий значению разряда.
    mov dl, byte [hex2char + edx]
    mov byte [resstr + ecx - 1], dl

    loop .loop_convert_hex2char

    ; Выводим результат операции.
    mov eax, str_max_value_is
    call io_print_string

    mov eax, resstr
    call io_print_string
    call io_newline

    xor eax, eax
.exit:
    ret

section .rodata
str_enter_size          db `Enter array size: `,0
str_array_size_too_big  db `Array size is too big\n`, 0
str_max_value_is        db `Max value is: `, 0

section .bss
size    resd    1
index   resd    1
arr     resd    10
resstr  resb    9

section .data
hex2char    db `0123456789ABCDEF`

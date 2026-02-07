; Copyright 2025 Vladislav Aleinik
bits 32

; Директива extern обозначает внешние символы.
; Внешние символы разрешаются во время линковки.
extern io_print_dec, io_print_hex, io_get_dec, io_print_string, io_newline

; Секция .text содержит исполняемый код
section .text
global main
main:
    mov ebp, esp; for correct debugging
    ; Указатель на стековый фрейм необходим для корректной работы отладчика. 

    ; Вывод строки "Hello, world!"
    mov eax, hello ; eax = hello
    call io_print_string

    ; Вывод 32-битного шестнадцатеричного числа.
    ; io_print_hex принимает аргумент через eax
    mov eax, dword [var_c] ; eax = *var_c
    call io_print_hex
    call io_newline

    ; Вывод строки "Enter two numbers to be added:"
    mov eax, addstr ; eax = addstr
    call io_print_string

    ; Чтение 32-битного целого числа.
    ; Возвращаемое значение в регистре EAX
    call io_get_dec
    mov ebx, eax ; ebx = eax

    ; Возвращаемое значение в регистре EAX
    call io_get_dec
    add ebx, eax ; ebx += eax

    ; Вывод строки "The result is:"
    mov eax, rsltstr ; eax = rsltstr
    call io_print_string

    ; Вывод результата сложения
    mov eax, ebx ; eax = ebx
    call io_print_dec
    call io_newline

    ; return 0
    xor eax, eax ; eax = eax ^ eax
    ret

; Секция .rodata содержит неизменяемые инициализированные данные
section .rodata
hello:   db `Hello, world!\n`, 0
addstr:  db `Enter two numbers to be added:\n`, 0
rsltstr: db `The result is:\n`, 0

; Секция .data содержит изменяемые инициализированные данные.
section .data
var_a   db 0xAA                ; Байт данные
var_b   dw 0xAABB              ; Слово данных (16 бит)
var_c   dd 0xDEADBEEF          ; Двойное слово (32 бита)
var_d   dq 0xDEADBEEFB01DFACE  ; Четверное слово (64 бита)

; Повторение некоторого заданного паттерна.
pattern times 16 dw 0xABCD

; Секция .bss содержит изменяемые неинициализированные данные.
section .bss
bss_a   resb 20     ; Неинициализированная область размером  20 байт
bss_b   resw 20     ; Неинициализированная область размером  40 байт
bss_c   resd 20     ; Неинициализированная область размером  80 байт
bss_d   resq 20     ; Неинициализированная область размером 160 байт

; Copyright 2026 Vladislav Aleinik
bits 32

;=====================;
; Внешние зависимости ;
;=====================;

; stdio.h
extern printf, scanf, segfault

;=================;
; Функция printer ;
;=================;

%define     num_hi  dword [ebp + 12]
%define     num_lo  dword [ebp +  8]
global printer
printer:
    enter   12, 0

    push    num_hi
    push    num_lo
    push    printf_format
    call    printf

    leave
    ret
%undef  num_hi
%undef  num_lo

;==============;
; Функция main ;
;==============;

section .text
%define     num_hi  dword [ebp -  8]
%define     num_lo  dword [ebp - 12]
global main
main:
    ; Учитываем выравнивание стека в инструкции enter
    enter   16, 0

    ; Оптимизированное выравнивание стека (перенесено в enter):
    ; sub   esp, 8
    lea     eax, num_lo
    push    eax 
    push    scanf_format
    call    scanf
    add     esp, 8

    cmp     eax, 1
    jne     .exit

    push    num_hi
    push    num_lo
    call    printer 
    add     esp, 16

    ; Предварительно ломаем выравнивание стека.
    sub     esp, 8
    ; Вызываем специальную функцию, для которой важно выравнивание стека.
    call    segfault

.exit:
    leave
    xor     eax, eax
    ret

section .rodata
printf_format: db `0x%016llx\n`, 0
scanf_format: db `%016llx`, 0
bits 32

;=====================;
; Внешние зависимости ;
;=====================;

; stdio.h
extern printf, scanf

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

;==============;
; Функция main ;
;==============;

section .rodata
scanf_format:   db `%f`, 0
printf_format:  db `%f`, 0xA, 0

section .text
global main
%define     x       dword [ebp -  4]
%define     y       qword [ebp -  12]
%define     y_hi    dword [ebp -  8]
%define     y_lo    dword [ebp -  12]
main:
    ; Аллоцируем стековый фрейм.
    FUNCTION_PROLOGUE 12

    ALIGN_STACK 8
    lea     eax, x
    push    eax
    push    scanf_format
    call    scanf
    UNALIGN_STACK 8

    ; Вычисляем y = exp(x)

    fldl2e
    ; ST(0) = log2(e)

    fmul    x
    ; ST(0) = x * log2(e)

    fld1
    ; ST(1) = x * log2(e)
    ; ST(0) = 1.0

    fld     st1
    ; ST(2) = x * log2(e)
    ; ST(1) = 1.0
    ; ST(0) = x * log2(e)

    fprem
    ; ST(2) = x * log2(e)
    ; ST(1) = 1.0
    ; ST(0) = rem = {x * log2(e)}

    f2xm1
    ; ST(2) = x * log2(e)
    ; ST(1) = 1.0
    ; ST(0) = 2^rem - 1 = 2^{x * log2(e)} - 1

    faddp
    ; ST(1) = x * log2(e)
    ; ST(0) = 2^{x * log2(e)}

    fscale
    ; ST(1) = x * log2(e)
    ; ST(0) = 2^[x * log2(e)] * 2^{x * log2(e)} = 2^(x * log2(e)) = e^x

    fstp y

    ALIGN_STACK 8
    mov     eax, y_hi
    push    eax
    mov     eax, y_lo
    push    eax
    push    printf_format
    call    printf
    UNALIGN_STACK 8

    ; return 0
    xor     eax, eax
.exit:
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ret

%undef x
%undef y

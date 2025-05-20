bits 32

;==============================;
; Requested external functions ;
;==============================;

; stdio.h
extern printf, scanf
; stdlib.h
extern calloc, free
; string.h
extern memcpy

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

;===============;
; Функция apply ;
;===============;

;==============================================================================
; Function: apply
; Description: applies function with list of arguments
;              to each array element
;==============================================================================
; Параметры функции (от последнего к первому):
;   int                 ...     (in)        - список из nargs 4-байтных аргументов
;   int                 nargs   (in)        - кол-во дополнительных аргументов
;   void (*fn) (...)    fn      (in)        - применяемая функция
;   size_t              length  (in)        - размер массива
;   int*                array   (in/out)    - массив элементов
; Возвращаемое значение: отсутствует
;==============================================================================
section .text
global apply
%define     arglist         dword [ebp + 24]
%define     nargs           dword [ebp + 20]
%define     fn              dword [ebp + 16]
%define     length          dword [ebp + 12]
%define     array           dword [ebp +  8]
%define     tmp_ebx         dword [ebp -  4]
%define     tmp_edi         dword [ebp -  8]
%define     tmp_esi         dword [ebp - 12]
%define     arglist_moved   dword [ebp - 16]
apply:
    ; Создаём стековый фрейм.
    FUNCTION_PROLOGUE 16
    ; Сохраняем регистры на стеке.
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi

    ;===================================;
    ; Подготовка стека к вызову функции ;
    ;===================================;

    ; Вычисляем размер зоны аргументов
    ; EBX := размер n элементов
    mov     ebx, nargs
    imul    ebx, ebx, 4

    ; Подготавливаем пространство для аргументов функции fn.
    sub     esp, 4
    sub     esp, ebx
    and     esp, 0xfffffff0

    ; Вычисояем адрес начала зоны аргументов функции fn.
    mov     arglist_moved, esp

    ; Применяем функцию fn к каждому элементу массива.
    ; ESI = индекс элемента в массиве.
    ; EDI = указатель на массив.
    mov     esi, 0
    mov     edi, array
.apply_loop:
    cmp     esi, length
    je      .apply_loop_exit

    ; Копируем nargs аргументов в зону аргументов
    ALIGN_STACK 12
    push    ebx                 ; Количество коируемых байт
    lea     eax, arglist        ; <
    push    eax                 ; < Откуда копируем
    push    arglist_moved       ; Куда копируем
    call    memcpy
    UNALIGN_STACK 12

    ; Возвращаем стек в состояние до вызова memcpy
    mov     esp, arglist_moved

    ; Сохраняем текущий элемент массива для передачи в функцию fn.
    mov     eax, arglist_moved
    add     eax, ebx
    mov     ecx, dword [edi]
    mov     dword [eax], ecx

    ; Производим вызов функции
    ; Примечание: стек уже выровнен и содержит все необходимые аргументы функции.
    mov     eax, fn
    call    eax

    add     esi, 1
    add     edi, 4
    jmp     .apply_loop
.apply_loop_exit:

    ; Восстанавливаем регистры
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Восстанваливаем состояние стека до вызова функции.
    FUNCTION_EPILOGUE
    ; Возвращаемся из процедуры.
    ret

; Избавляемся от макросов.
%undef      arglist
%undef      nargs
%undef      fn
%undef      length
%undef      array
%undef      tmp_ebx
%undef      tmp_edi
%undef      tmp_esi
%undef      elem_ptr

;==============;
; Функция main ;
;==============;

section .text
global main
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
%define     array_size  dword [ebp - 16]
%define     array_ptr   dword [ebp - 20]
main:
    FUNCTION_PROLOGUE 20
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi

    ;=============================;
    ; Выделение памяти под массив ;
    ;=============================;

    ; Выводим строку для пользователя:
    ALIGN_STACK 4
    push    str_prompt_array_size   ; строка
    call    printf
    UNALIGN_STACK 4

    ; Вводим размер массива.
    ; Предположение: размер массива корректен.
    ALIGN_STACK 8
    lea     ebx, array_size         ; <
    push    ebx                     ; < указатель на переменную
    push    str_format_array_size   ; форматная строка
    call    scanf
    UNALIGN_STACK 8

    ; Выделяем память под вводимый массив.
    ; Предположение: выделение памяти проходит успешно.
    ALIGN_STACK 8
    push    4           ; размер элемента массива
    push    array_size  ; кол-во элементов
    call    calloc
    UNALIGN_STACK 8

    mov     array_ptr, eax

    ;========================;
    ; Ввод элементов массива ;
    ;========================;

    ; Выводим строку для пользователя.
    ALIGN_STACK 4
    push    str_prompt_array_elements   ; строка
    call    printf
    UNALIGN_STACK 4

    mov     ebx, 0
    mov     edi, array_ptr
.enter_array_loop:
    cmp     ebx, array_size
    je      .enter_array_loop_exit

    ; Вводим элемент массива.
    ; Предположение: введён валидный элемент массива.
    ALIGN_STACK 8
    push    edi                         ; указатель на переменную
    push    str_format_array_element    ; форматная строка
    call    scanf
    UNALIGN_STACK 8

    add     ebx, 1
    add     edi, 4
    jmp     .enter_array_loop
.enter_array_loop_exit:

    ;========================;
    ; Вызываем функцию apply ;
    ;========================;

    ALIGN_STACK 20
    push    str_format_array_printout ; arglist[0]
    push    1                         ; nargs
    push    printf                    ; fn
    push    array_size                ; length
    push    array_ptr                 ; array
    call    apply
    UNALIGN_STACK 20

.exit:
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    FUNCTION_EPILOGUE
    xor     eax, eax
    ret

%undef     tmp_ebx
%undef     tmp_edi
%undef     tmp_esi
%undef     array_size
%undef     array_ptr

section .rodata
str_prompt_array_size:      db `Enter array size:\n`, 0
str_format_array_size:      db `%u`, 0
str_prompt_array_elements:  db `Enter array elements:\n`, 0
str_format_array_element:   db `%d`, 0
str_format_array_printout:  db `%d\n`, 0

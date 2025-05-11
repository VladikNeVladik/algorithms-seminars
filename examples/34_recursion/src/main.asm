bits 32

extern io_get_udec, io_get_char
extern io_print_udec, io_print_char, io_print_dec
extern io_print_string, io_newline

;========================;
; Функциональные макросы ;
;========================;

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

;====================;
; Функция сортировки ;
;====================;

;----------------------------------------------------------
; Функция: swap
; Назначение: Меняет местами два объекта заданного размера.
;----------------------------------------------------------
; Параметры:
;   ptr1 (in/out)   - указатель на первый объект
;   ptr2 (in/out)   - указатель на второй объект
;   size (in)       - размер объекта
; Возвращаемое значение: отсутствует
;----------------------------------------------------------
global      swap
%define     size        dword [ebp + 16]
%define     ptr2        dword [ebp + 12]
%define     ptr1        dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
swap:
    ; Инициализируем стековый фремй
    FUNCTION_PROLOGUE 12
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Инициализируем индексы.
    mov     ebx, ptr1
    mov     edx, ptr2
    mov     ecx, size

.loop_copy:
    mov    al, byte [ebx]
    mov    ah, byte [edx]

    mov    byte [ebx], ah
    mov    byte [edx], al

    add    ebx, 1
    add    edx, 1
    loop  .loop_copy
.exit:

    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef  size
%undef  ptr2
%undef  ptr1
%undef  tmp_ebx
%undef  tmp_edi
%undef  tmp_esi

;----------------------------------------------------------
; Функция: sort
; Назначение: сортирует переданный массив данных
;----------------------------------------------------------
; Параметры:
;   base   (in/out) - указатель на начало массива
;   num    (in)     - количество элементов
;   size   (in)     - размер элемента массива
;   compar (in)     - компаратор
; Возвращаемое значение: отсутствует
;----------------------------------------------------------
global sort
%define     compar      dword [ebp + 20]
%define     size        dword [ebp + 16]
%define     num         dword [ebp + 12]
%define     base        dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
%define     tmp_ecx     dword [ebp - 16]
%define     tmp_edx     dword [ebp - 20]
sort:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 20
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Массив, содержащий 0 или 1 элементов, уже отсортирован.
    cmp     num, 1
    jbe     .exit

    ; Выбираем последний элемент как опорный.
    mov     ecx, num
    sub     ecx, 1

    ; Назначение регистров:
    ; EDX = индекс левого элемента
    ; EBX = &array[EDX]
    ; ECX = индекс опорного элемента
    ; ESI = индекс правого элемента
    ; EDI = &array[ESI]
    mov     edx, 0
    mov     ebx, base
    mov     esi, 0
    mov     edi, base

    ; Будем честны, этот алгоритм возможно прочитать только после чтения его псевдокода.
    ; https://en.wikipedia.org/wiki/Quicksort#Lomuto_partition_scheme
.loop_partition:
    cmp     edx, ecx
    je      .loop_partition_exit

    ; Вычисляем указатель на опорный элемент.
    ; EAX = &array[ECX]
    mov     eax, ecx
    imul    eax, size
    add     eax, base

    ; Сохраняем регистры, перезаписываемые вызываемой функцией.
    mov     tmp_ecx, ecx
    mov     tmp_edx, edx

    ; Вызываем компаратор по указателю на функцию.
    ; compar(указатель на левый элемент, указатель на опорный элемент)
    ALIGN_STACK 8
    push    eax
    push    ebx
    call    compar
    UNALIGN_STACK 8

    ; Производим переход, если левый элемент меньше опорного.
    cmp     eax, 0
    jl      .do_not_swap

    ; Меняем местами левый элемент и опорный элемент.
    ALIGN_STACK 12
    push    size
    push    ebx
    push    edi
    call    swap
    UNALIGN_STACK 12

    ; Перемещаем левый индекс на один вперёд.
    add     esi, 1
    add     edi, size
    .do_not_swap:

    ; Восстанавливаем регистры после вызовов функций.
    mov     ecx, tmp_ecx
    mov     edx, tmp_edx

    ; Обновляем индексы.
    add     edx, 1
    add     ebx, size
    jmp     .loop_partition
.loop_partition_exit:

    ; Получаем указатель на последний элемент.
    mov     eax, ecx
    mul     size
    add     eax, base

    ; Меняем опорный элемент и последний элемент.
    ALIGN_STACK 12
    push    size
    push    eax
    push    edi
    call    swap
    UNALIGN_STACK 12

    ; Рекурсивно сортируем подмассивы.
.sort_first_subarray:
    cmp     esi, 1
    jl      .sort_second_subarray

    ; sort(base, esi - 1, size, compar)
    mov     eax, esi
    sub     eax, 1

    ALIGN_STACK 16
    push    compar
    push    size
    push    eax
    push    base
    call    sort
    UNALIGN_STACK 16

.sort_second_subarray:
    mov     ebx, esi
    add     ebx, 1

    cmp     ebx, num
    jge     .exit

    ; sort(base + (esi+1)*size, num - (esi+1), size, compar):
    mov     eax, ebx
    imul    eax, size
    add     eax, base

    mov     ecx, num
    sub     ecx, ebx

    ALIGN_STACK 16
    push    compar
    push    size
    push    ecx
    push    eax
    call    sort
    UNALIGN_STACK 16

    ; Массив отсортирован!

.exit:
    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi

    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret
%undef  compar
%undef  size
%undef  num
%undef  base
%undef  tmp_ebx
%undef  tmp_edi
%undef  tmp_esi
%undef  tmp_ecx
%undef  tmp_edx

;========================;
; Сортируемый тип данных ;
;========================;

; Объявление структуры.
struc rational
    .numerator      resd 1
    .denominator    resd 1
endstruc

; Своеобразное объявление перечисления (enum).
INPUT_FINISHED      equ 0
INPUT_CONTINUED     equ 1
INPUT_ERROR         equ 2

;----------------------------------------------------------
; Функция: rational_input
; Назначение: читает рациональное число из стандартного
; потока ввода.
;----------------------------------------------------------
; Параметры:
;   rslt_ptr (out) - указатель на результирующее
;                    рациональное число.
; Возвращаемое значение:
;   EAX  - флаг с указанием текущего статуса ввода.
;          Возможные значения:
;            INPUT_FINISHED  - ввод завершён.
;            INPUT_CONTINUED - ввод продолжается.
;            INPUT_ERROR     - неопознанный символ.
;----------------------------------------------------------
global rational_input
%define     rslt_ptr    dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
rational_input:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 12
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Сохраняем указатель на рациональное число EBX:
    mov     ebx, rslt_ptr

    ; Считываем числитель.
    call    io_get_udec
    mov     dword [ebx + rational.numerator], eax

    ; Инициализируем знаменатель.
    mov     dword [ebx + rational.denominator], 1

    ; Считываем символ разделителя.
    ; EAX = '/' or ',' or '.'
    call    io_get_char

    ; Определяем тип считанного символа.
    ; switch (EAX)
    ; {
    ; case '/':
.case_slash:
    cmp     eax, '/'
    jne     .case_comma

    ; Считываем знаменатель.
    call    io_get_udec
    mov     dword [ebx + rational.denominator], eax

    ; Считываем разделитель.
    ; EAX = ',' or '.'
    call    io_get_char

    ; Проваливаемся в следующую ветку switch.
    ; Ровно это же происходит в языке C, если опустить "break;".

    ; case ',':
.case_comma:
    cmp     eax, ','
    jne     .case_dot

    mov     eax, INPUT_CONTINUED
    jmp     .exit

    ; case '.':
.case_dot:
    cmp     eax, '.'
    jne     .case_default

    mov     eax, INPUT_FINISHED
    jmp     .exit

    ; default:
.case_default:
    mov     eax, INPUT_ERROR

    ; Вываливаемся из switch.
    ; Здесь "break;" не нужен.

.exit:
    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef  tmp_ebx
%undef  tmp_edi
%undef  tmp_esi
%undef  rslt_ptr

;----------------------------------------------------------
; Function: rational_print
; Description: prints a seperate rational number
;----------------------------------------------------------
; Parameters:
;   rat_ptr (in)    - pointer to the printed rational
; Return value: none
;----------------------------------------------------------
global rational_print
%define     rat_ptr     dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
rational_print:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 12
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Save rational pointer to EBX:
    mov     ebx, rat_ptr

.print_numerator:
    mov     eax, dword [ebx + rational.numerator]
    call    io_print_udec

    cmp     dword [ebx + rational.denominator], 1
    je     .do_not_print_denominator

.print_denominator:
    mov     eax, '/'
    call    io_print_char

    mov     eax, dword [ebx + rational.denominator]
    call    io_print_udec

.do_not_print_denominator:
    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef  rat_ptr
%undef  tmp_ebx
%undef  tmp_edi
%undef  tmp_esi

; Comparison results:
CMP_ABOVE       equ 1
CMP_EQUAL       equ 0
CMP_BELOW       equ -1

;----------------------------------------------------------
; Функция: rational_compare
; Назначение: сравнивает два рациональных числа.
;----------------------------------------------------------
; Параметры:
;   rat1_ptr (in)   - указатель на первое рациональное.
;   rat2_ptr (in)   - указатель на второе рациональное.
; Возвращаемое значение:
;   EAX - результат сравнения
;         Возможные значения:
;         - CMP_ABOVE (rat1  > rat2)
;         - CMP_EQUAL (rat1 == rat2)
;         - CMP_BELOW (rat1  < rat2)
;----------------------------------------------------------
global rational_compare
%define     rat2_ptr        dword [ebp + 12]
%define     rat1_ptr        dword [ebp +  8]
%define     tmp_ebx         dword [ebp -  4]
%define     tmp_edi         dword [ebp -  8]
%define     tmp_esi         dword [ebp - 12]
%define     rat2_val_u      dword [ebp - 16]
%define     rat2_val_l      dword [ebp - 20]
%define     rat1_val_u      dword [ebp - 24]
%define     rat1_val_l      dword [ebp - 28]
rational_compare:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 28
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ; Математика:
    ; rat1 = n1 / d1
    ; rat2 = n2 / d2
    ; rat1_val = n1*d2
    ; rat2_val = n2*d1
    ; result = sign(n1*d2 - n2*d2) = sign(rat1_val - rat2_val)

    ; Предзагружаем значения rat1 and rat2 указателей.
    mov     esi, rat1_ptr
    mov     edi, rat2_ptr

    ; Вычисляем rat1_val.
    mov     eax, dword [esi + rational.numerator]
    mul     dword [edi + rational.denominator]

    mov     rat1_val_u, edx
    mov     rat1_val_l, eax

    ; Вычисляем rat2_val.
    mov     eax, dword [edi + rational.numerator]
    mul     dword [esi + rational.denominator]

    mov     rat2_val_u, edx
    mov     rat2_val_l, eax

.cmp_upper:
    ; Предзагружаем аргументы инструкции CMOV.
    mov     ebx, CMP_EQUAL
    mov     esi, CMP_BELOW
    mov     edi, CMP_ABOVE

    ; Сравниваем старшие части.
    mov     ecx, rat1_val_u
    cmp     ecx, rat2_val_u
    je      .cmp_lower

    ; Устанавливаем возвращаемое значение.
    cmovb   eax, esi ; CMP_BELOW
    cmova   eax, edi ; CMP_ABOVE

    jmp     .exit

.cmp_lower:
    ; Сравниваем младшие части.
    mov     ecx, rat1_val_l
    cmp     ecx, rat2_val_l

    ; Устанавливаем возвращаемое значение.
    cmovb   eax, esi ; CMP_BELOW
    cmove   eax, ebx ; CMP_EQUAL
    cmova   eax, edi ; CMP_ABOVE

.exit:
    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret

; Удаляем макросы во избежание ошибок.
%undef  rat2_ptr
%undef  rat1_ptr
%undef  tmp_ebx
%undef  tmp_edi
%undef  tmp_esi
%undef  rat2_val_u
%undef  rat2_val_l
%undef  rat1_val_u
%undef  rat1_val_l


;===============;
; Main function ;
;===============;

section .text
global main
%define     tmp_ebx         dword [ebp -  4]
%define     tmp_edi         dword [ebp -  8]
%define     tmp_esi         dword [ebp - 12]
main:
    ; Инициализируем стековый фрейм
    FUNCTION_PROLOGUE 12
    ; Сохраняем callee-preserved регистры.
    mov     tmp_ebx, ebx
    mov     tmp_edi, edi
    mov     tmp_esi, esi

    ;==============================;
    ; Вводим массив для сортировки ;
    ;==============================;

    ; Используем странную схему аллокации памяти на стеке:
    ; - Аллоцируем место под значения по одному.
    ; - Такой подход к аллокации памяти на стеке нереализуем в языке C.

    ; ESI = размер массива
    ;
    ; NOTE: do not handle zero-sized input arrays correctly.
    mov     esi, 0

.loop_input_array_element:
    ; Аллоцируем одну структуру на стеке.
    sub     esp, 8

    ; Сохраняем текущее значение указателя на начало массива.
    ; EBX = начало массива
    mov     ebx, esp

    ; Вводим рациональное число с консоли.
    ALIGN_STACK 4
    push    ebx
    call    rational_input
    UNALIGN_STACK 4

    ; Восстанавливаем значение вершины стека (оно потерялось после выравнивания).
    mov     esp, ebx

    ; Определяем результат ввода.
    ; switch (EAX)
    ; {
    .case_input_error:
        cmp     eax, INPUT_ERROR
        jne     .case_input_continued

        mov     eax, str_input_error
        call    io_print_string

        ; exit(EXIT_FAILURE);
        mov     eax, 1
        jmp     .exit

    .case_input_continued:
        add     esi, 1

        cmp     eax, INPUT_CONTINUED
        je     .loop_input_array_element

    .case_input_finished:
    ; }

.loop_input_array_element_exit:

    ; В данной точке программы:
    ; EBX = указатель на начало массива.
    ; ESI = количество элементов.
    ; Состояние стека:
    ;   [EBX + 8n] array[n]
    ;   [EBX +  8] array[1]
    ;   [EBX +  0] array[0] (последний введённый элемент)

    ; Сортируем массив.
    ALIGN_STACK 16
    push    rational_compare
    push    8
    push    esi
    push    ebx
    call    sort
    UNALIGN_STACK 16

    ;===================;
    ; Выводим результат ;
    ;===================;

    ; Не печатем пустой массив.
    cmp     esi, 0
    je      .loop_print_result_exit

    ; EDI = текущий индекс.
    mov     edi, esi
.loop_print_result:
    sub     edi, 1

    ; Вычисляем адрес в массиве.
    lea     eax, byte [ebx + edi * 8]

    ; Выводим действительное число.
    ALIGN_STACK 4
    push    eax
    call    rational_print
    UNALIGN_STACK 4

    ; Выводим текущий разделитель.
    mov     ecx, '.'
    mov     edx, ','

    cmp     edi, 0
    cmove   eax, ecx
    cmovne  eax, edx
    call    io_print_char

    ; Обрабатываем выход из цикла.
    cmp     edi, 0
    jne     .loop_print_result
.loop_print_result_exit:

    ; Выводим символ переноса строки.
    call io_newline

    ; Операция "return 0".
    xor     eax, eax
.exit:
    ; Восстанавливаем callee-preserved регистры.
    mov     ebx, tmp_ebx
    mov     edi, tmp_edi
    mov     esi, tmp_esi
    ; Восстанавливаем стековый фрейм.
    FUNCTION_EPILOGUE
    ; Возвращаемся из функции.
    ret
; Удаляем макросы во избежание ошибок.
%undef  tmp_ebx
%undef  tmp_edi
%undef  tmp_esi

section .rodata
str_input_error:         db `Input error\n`, 0

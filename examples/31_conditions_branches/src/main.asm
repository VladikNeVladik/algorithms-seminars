; Copyright 2025 Vladislav Aleinik
bits 32

extern io_print_udec, io_get_udec, io_print_string, io_newline

section .text
global main
main:
    mov ebp, esp; for correct debugging
    ; Указатель на стековый фрейм необходим для корректной работы отладчика. 

    ; call example_conditionals_asm
    ; call jumps

    ;====================;
    ; Операции сравнения ;
    ;====================;

    ; eax = *u32_a
    ; ebx = *u32_b
    mov     eax, dword [u32_a]
    mov     ebx, dword [u32_b]

    ; Инструкция сравнения.
    cmp     eax, ebx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ; Инструкция вычитания.
    sub     eax, ebx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ;===================;
    ; Вычисление модуля ;
    ;===================;

    ; Задача #1: вычислить |eax| при помощи условного ветвления.
    mov     eax, dword [s32_c]

    cmp     eax, 0
    jge     .do_not_negate
    neg     eax
    .do_not_negate:

    mov     dword [s32_d], eax

    mov     eax, dword [s32_c]

    ; ebx = -eax
    mov     ebx, 0
    sub     ebx, eax

    cmp     eax, 0
    cmovl   eax, ebx

    mov     dword [s32_d], eax

    ; Задача #3: вычислить |eax| при помощи инструкции SAR.
    mov     eax, dword [s32_c]
    mov     ebx, eax
    sar     ebx, 31     ; 11111111 (-1) or 00000000 (0)

    shl     ebx, 1      ; -2 или 0
    imul    ebx, eax    ; -2*eax или 0
    add     ebx, eax    ; -eax или +eax

    mov     dword [s32_d], ebx

    ;==============================;
    ; Рассчёт N-го числа Фибоначчи ;
    ;==============================;

    ; Выводим приветственную строку.
    mov     eax, inputstr
    call    io_print_string

    ; Считываем номер числа Фибоначчи для рассчёта.
    call    io_get_udec
    ; ESI - номер итогового числа Фибоначчи
    mov     esi, eax

    ; Выводим нулевое число Фибоначии
    cmp     esi, 0
    jne     .init_fibs_computation

    mov     eax, 0
    call    io_print_udec
    call    io_newline
    jmp     .exit

.init_fibs_computation:
    ; ECX - номер текущего числа Фибоначчи
    mov     ecx, 0
    ; EDI - предыдущее число Фибоначчи
    mov     edi, 0
    ; EBX - текущее число Фибоначчи
    mov     ebx, 1
.compute_next_fibs:
    ; EDI := EBX
    ; EBX := EDI + EBX
    add     edi, ebx
    xchg    edi, ebx

    ; Производим проверку на переполнение.
    jc      .detect_overflow

    inc     ecx
    cmp     ecx, esi
    jl     .compute_next_fibs

    ; Выводим результирующее число Фибоначчи.
    mov     eax, outputstr
    call    io_print_string

    mov     eax, edi
    call    io_print_udec
    call    io_newline
    jmp     .exit

.detect_overflow:
    mov     eax, overflowstr
    call    io_print_string

.exit:
    xor     eax, eax
    ret

example_conditionals_asm:
    ; Пример #1
    mov     eax, dword [u32_a]
    mov     ebx, dword [u32_b]
    cmp     eax, ebx
    jbe     .not_a_above_b
    ret
.not_a_above_b:

    ; Пример #2
    mov     eax, dword [u32_a]
    mov     ebx, dword [u32_b]
    cmp     eax, ebx
    je      .not_a_neq_b
    ret
.not_a_neq_b:

    ; Пример #3
    mov     eax, dword [u32_a]
    mov     ebx, dword [u32_b]
    cmp     eax, ebx
    jg      .skip_if3

    mov     ecx, dword [s32_c]
    mov     edx, dword [s32_d]
    cmp     ecx, edx
    jl      .skip_if3
    ret
.skip_if3:
    
    ; Пример #4
    mov     ecx, dword [s32_c]
    mov     edx, dword [s32_d]
    cmp     ecx, edx
    jne     .not_c_eq_d
    ret
.not_c_eq_d:

    ; Пример #5
    mov     ecx, dword [s32_c]
    mov     edx, dword [s32_d]
    cmp     ecx, edx
    jle     .not_c_gt_d
    ret
.not_c_gt_d:

    ; Пример #6
    mov     ecx, dword [s32_c]
    mov     edx, dword [s32_d]
    cmp     ecx, edx
    jge     .enter_if6

    mov     eax, dword [u32_a]
    mov     ebx, dword [u32_b]
    cmp     eax, ebx
    ja      .enter_if6

    jmp     .skip_if6
.enter_if6:
    ret
.skip_if6:

    xor eax, eax
    ret

jumps:
    jmp .forward
.backward:
    nop
    nop
    nop
.forward:
    inc eax
    xor ecx, ecx
    jmp .backward

    xor eax, eax
    ret

section .rodata
inputstr:       db `Enter a number: `, 0
outputstr:      db `Fibinacci number is: `, 0
overflowstr:    db `Detected overflow!`, 0xA, 0

section .data
u32_a   dd      10
u32_b   dd      15

s32_c   dd      -204
s32_d   dd      0

bits 32

extern io_print_udec, io_get_udec, io_print_string, io_newline

section .text
global main
main:
    mov ebp, esp; for correct debugging
    ; Указатель на стековый фрейм необходим для корректной работы отладчика. 

    ;====================;
    ; Операции сравнения ;
    ;====================;

    ; eax = *u32_a
    ; ebx = *u32_b
    mov eax, dword [u32_a]
    mov ebx, dword [u32_b]

    ; Инструкция сравнения.
    cmp eax, ebx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ; Инструкция вычитания.
    sub eax, ebx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

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
    jne     init_fibs_computation

    mov     eax, 0
    call    io_print_udec
    call    io_newline
    jmp     exit

init_fibs_computation:
    ; ECX - номер текущего числа Фибоначчи
    mov     ecx, 0
    ; EDI - предыдущее число Фибоначчи
    mov     edi, 0
    ; EBX - текущее число Фибоначчи
    mov     ebx, 1
compute_next_fibs:
    ; EDI := EBX
    ; EBX := EDI + EBX
    add     edi, ebx
    xchg    edi, ebx

    ; Производим проверку на переполнение.
    jc      detect_overflow

    inc     ecx
    cmp     ecx, esi
    jl     compute_next_fibs

    ; Выводим результирующее число Фибоначчи.
    mov     eax, outputstr
    call    io_print_string

    mov     eax, edi
    call    io_print_udec
    call    io_newline
    jmp     exit

detect_overflow:
    mov     eax, overflowstr
    call    io_print_string

exit:
    xor eax, eax
    ret

section .rodata
inputstr:       db `Enter a number: `, 0
outputstr:      db `Fibinacci number is: `, 0
overflowstr:    db `Detected overflow!`, 0xA, 0

section .data
u32_a   dd      10
u32_b   dd      15

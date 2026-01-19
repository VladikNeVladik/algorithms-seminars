bits 32

extern io_print_dec, io_print_hex, io_get_dec, io_print_string, io_newline
extern assembled_example_function

; Размещаем неинициализированные статические переменные в секции .bss
section .bss
var_08bit:  resb 1 ; Резервирование 1 байта
var_16bit:  resw 1 ; Резервирование 1 слова (16 бит)
var_32bit:  resd 1 ; Резервирование 1 двойного слова (32 бита)

res0:       resd 1
res1:       resd 1
res2:       resd 1
res3:       resd 1

section .data
buffer:     db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

section .rodata
ushifted:   dd 10d
sshifted:   dd 10d

section .text
global main
main:
    mov ebp, esp; for correct debugging
    ; Указатель на стековый фрейм необходим для корректной работы отладчика. 

    ; Вызов кода на C для сравнения результата.
    call assembled_example_function

    ; Представление переменных разного размера.
    mov     al, 01h
    mov     byte [var_08bit], al

    mov     bx, 0001h
    mov     word [var_16bit], bx

    mov     ecx, 00000001h
    mov     dword [var_32bit], ecx

    ; Целочисленная арифметика.
    mov     al, byte [var_08bit]
    add     al, 1h
    mov     byte [var_08bit], al

    mov     ax, word [var_16bit]
    sub     ax, 1h
    mov     word [var_16bit], ax

    mov     eax, dword [var_32bit]
    mov     ebx, 10
    imul    ebx ; edx:eax = eax * ebx
    mov     dword [var_32bit], eax

    mov     eax, dword [var_32bit]
    mov     edx, eax
    sar     edx, 31

    mov     ebx, 5h
    idiv    ebx ; edx = edx:eax % ebx, eax = edx:eax / ebx
    mov     dword [var_32bit], eax

    ; Вывод результатов арифметических операций.
    movzx   eax, byte [var_08bit]
    call    io_print_hex
    call    io_newline

    movzx   eax, word [var_16bit]
    call    io_print_hex
    call    io_newline

    mov     eax, dword [var_32bit]
    call    io_print_hex
    call    io_newline

    ; Доступы в массив.
    mov     byte [buffer], Ah

    mov     al, byte [buffer + 1]
    mov     byte [var_08bit], al

    ; Битовые сдвиги.
    mov     eax, dword [ushifted]
    shl     eax, 10d
    mov     dword [res0], eax

    mov     eax, dword [ushifted]
    shr     eax, 10d
    mov     dword [res0], eax

    mov     eax, dword [sshifted]
    sal     eax, 10d
    mov     dword [res0], eax

    mov     eax, dword [sshifted]
    sar     eax, 10d
    mov     dword [res0], eax

    ; Успешный Возврат из функции (return 0).
    xor     eax, eax ; eax = eax ^ eax
    ret

; Задания для выполнения на семинаре.
section .data
a:  db 0x40
    dw 0x81
    db 0xFE
b:  dd 0x0

section .text
global tasks
tasks:
    ; Задача #1
    mov     eax, dword [a]
    mov     dword [b], eax
    ; b = ?

    ; Задача #2
    movsx   eax, byte [a + 3]
    mov     dword [b], eax
    ; b = ?

    ; Задача #3
    movsx   eax, word [a + 1]
    mov     dword [b], eax
    ; b = ?

    ; Задача #4
    movsx   eax, byte [a + 1]
    mov     dword [b], eax
    ; b = ?

    ; Задача #5
    movzx   eax, byte [a + 1]
    mov     dword [b], eax
    ; b = ?

    ; Задача #6
    xor     eax, eax
    movsx   ax, byte [a + 1]
    mov     dword [b], eax
    ; b = ?

    ;  Успешный Возврат из функции (return 0).
    xor     eax, eax ; eax = eax ^ eax
    ret

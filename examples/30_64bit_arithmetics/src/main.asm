bits 32

extern io_print_dec, io_print_hex, io_get_dec, io_print_string, io_newline

section .text
global main
main:
    ;=========================================;
    ; Обработка переполнения и регистр EFLAGS ;
    ;=========================================;

    ; Signed Flag (SF), Carry Flag (CF):
    mov ah, 0ffh
    mov al, 0feh
    add ah, al
    ; CFLAGS: SF=1 ZF=0 OF=0 CF=1

    ; Статусные флаги не выставляются.
    mov ah, 0ffh
    mov al, 0feh
    sub ah, al
    ; CFLAGS: SF=0 ZF=0 OF=0 CF=0

    mov ax, 0FFFFh
    mov dx,    01h
    add ax, dx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    mov ax, 07FFFh
    mov dx, 07FFFh
    add ax, dx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    mov ax, 01000h
    mov dx, 01000h
    add ax, dx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ;======================;
    ; 64-битная арифметика ;
    ;======================;

    ; NOTE: Архитектура x86 имеет инвертированный порядок байт (little endian)
    ; u64_a   = 0x0123456789ABCDEF
    ; ADDRESS = [00][01][02][03][04][05][06][07]
    ; DATA    = [EF][CD][AB][89][67][45][23][01]

    ; edx:eax = qword [u64_a]
    mov     eax, dword [u64_a + 0]
    mov     edx, dword [u64_a + 4]

    ; ecx:ebx = qword [var64bit_b]
    mov     ebx, dword [u64_b + 0]
    mov     ecx, dword [u64_b + 4]

    ; eax = eax + ebx
    ; Выставляем флаг переноса CF (Carry Flag)
    add     eax, ebx

    ; edx = edx + ecx + CF
    ; Используем флаг переноса CF
    adc     edx, ecx

    ; qword [var64bit_a] = edx:eax
    mov     dword [u64_a + 0], eax
    mov     dword [u64_a + 4], edx

    ;======================;
    ; Условные перемещения ;
    ;======================;

    ; Считываем два числа из консоли.
    call io_get_dec
    mov ebx, eax

    call io_get_dec
    mov ecx, eax

    ; Производим сравнение.
    cmp ebx, ecx

    ; Выполняем условное перемещение.
    cmovl eax, dword [cmp_neg]
    cmove eax, dword [cmp_zer]
    cmovg eax, dword [cmp_pos]

    ; Print result:
    call io_print_dec

    ;===================;
    ; Вычисление модуля ;
    ;===================;

    ; Задача #1: вычислить |eax| при помощи условного вычисления.
    mov eax, dword [s32_c]

    ; ebx = -eax
    mov ebx, 0
    sub ebx, eax

    cmp eax, 0
    cmovl eax, ebx

    ; Задача #2: вычислить |eax| при помощи инструкции SAR

    mov ebx, eax
    sar ebx, 31 ; 11111111 (-1) or 00000000 (0)

    shl ebx, 1   ; -2 или 0
    imul ebx, eax ; -2*eax или 0
    add ebx, eax  ; -eax или +eax

    xor eax, eax
    ret

section .data
u64_a:     dq 0x0123456789ABCDEF
u64_b:     dq 0x1010101010101010

s32_a:     dd -2
s32_b:     dd -2
s32_c:     dd 0
s32_d:     dd 0

u32_a:     dd 2
u32_b:     dd 2
u32_c:     dd 0
u32_d:     dd 0

cmp_pos:    dd +1
cmp_zer:    dd 0
cmp_neg:    dd -1

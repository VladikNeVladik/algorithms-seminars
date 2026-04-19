; Copyright 2026 Vladislav Aleinik
bits 32

extern malloc, free

;===================================================;
; Функции для работы с кольцевым двусвязным списком ;
;===================================================;

NULL equ 0
NODE_SIZE equ 12

struc Node
    .value  resd 1
    .next   resd 1
    .prev   resd 1
endstruc

section .text
%define     queue   dword [esp + 4]
global queue_init
queue_init:
    mov     ecx, queue
    mov     dword [ecx + Node.next], ecx
    mov     dword [ecx + Node.prev], ecx
    ret
%undef queue

%define     queue   dword [esp + 16]
%define     tmp_esi dword [esp + 8]
%define     tmp_ebx dword [esp + 4]
global queue_free
queue_free:
    sub     esp, 12
    mov     tmp_esi, esi
    mov     tmp_ebx, ebx

    ; EBX = root = указатель на корневой узел.
    mov     ebx, queue

    ; ESI = node = root->next
    mov     esi, dword [ebx + Node.next]
    cmp     esi, edx
    je      .free_loop_end

.free_loop:
    ; Готовим аргументы для вызова free.
    mov     dword [esp], esi

    ; node = node->next
    mov esi, dword [esi + Node.next]

    ; Освобождаем выбранный узел связного списка.
    call    free

    cmp     esi, ebx
    jne     .free_loop  
.free_loop_end:

    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    add     esp, 12
    ret
%undef queue

%define     data    dword [esp + 20]
%define     queue   dword [esp + 16]
global queue_add_tail
queue_add_tail:
    sub     esp, 12
    
    ; Пробуем выделить узел связного списка.
    ; Node* new = malloc(sizeof(Node))
    mov     dword [esp], NODE_SIZE
    call    malloc
    ; EAX = new = указатель на новый узел

    ; Проверяем успещность выделения памяти.
    test    eax, eax
    jnz     .malloc_successful
    ; return -1
    mov     eax, -1
    jmp     .exit
.malloc_successful:

    ; ECX = root = указатель на корневой узел.
    mov     ecx, queue
    ; EDX = tail = указатель на хвостовой узел.
    mov     edx, dword [ecx + Node.prev]

    ; Связываем новый узел с хвостовым и корневым узлами.
    ; new->next = root
    mov     dword [eax + Node.next], ecx
    ; new->prev = tail
    mov     dword [eax + Node.prev], edx

    ; Связываем корневой и хвостовой узлы с новым узлом.
    ; root->prev = new
    mov     dword [ecx + Node.prev], eax
    ; tail->next = new
    mov     dword [edx + Node.next], eax

    ; Записываем данные нового узла.
    ; new->value = data
    mov     ecx, data
    mov     dword [eax + Node.value], ecx

    ; return 0
    xor     eax, eax
.exit:
    add     esp, 12
    ret
%undef queue
%undef data

%define     data_ptr    dword [esp + 20]
%define     queue       dword [esp + 16]
global queue_remove_head
queue_remove_head:
    sub     esp, 12
    
    ; EAX = root = указатель на корневой узел.
    mov     eax, queue
    ; ECX = head = указатель на головной узел.
    mov     ecx, dword [eax + Node.next]

    cmp     eax, ecx
    jne     .queue_not_empty
    ; return -1
    mov     eax, -1
    jmp     .exit
.queue_not_empty:

    ; Удаляем голову из списка.
    ; root->next = head->next
    mov     edx, dword [ecx + Node.next]
    mov     dword [eax + Node.next], edx

    ; head->next->prev = root
    mov     edx, dword [ecx + Node.next]
    mov     dword [edx + Node.prev], eax

    ; Сохраняем данные из головы списка.
    ; *data = head->value
    mov     eax, data_ptr
    mov     edx, dword [ecx + Node.value]
    mov     dword [eax], edx

    ; Освобождаем память головного узла.
    ; free(head)
    mov     dword [esp], ecx
    call    free

    xor     eax, eax
.exit:
    add     esp, 12
    ret
%undef queue
%undef data_ptr

%define     queue   dword [esp + 4]
global queue_empty
queue_empty:
    ; EAX = root = указатель на корневой узел.
    mov     eax, queue
    ; EDX = head = указатель на головной узел.
    mov     edx, dword [eax + Node.next]

    ; return head == root
    mov     ecx, 0
    cmp     eax, edx
    cmovne  eax, ecx 
    ret
%undef queue

%define     addval  dword [esp + 20]
%define     mulval  dword [esp + 16]
%define     queue   dword [esp + 12]
global queue_scale_value
queue_scale_value:
    push    ebx
    push    esi

    ; EAX = mulval
    mov     eax, mulval
    ; EBX = addval
    mov     ebx, addval

    ; ESI = root = указатель на корневой узел.
    mov     esi, queue

    ; ECX  = node = указатель на текущий узел.
    ; node = root->next
    mov     ecx, dword [esi + Node.next]
    cmp     ecx, esi
    je      .scale_loop_end

.scale_loop:
    mov     edx, dword [ecx + Node.value]
    imul    edx, eax
    add     edx, ebx
    mov     dword [ecx + Node.value], edx

    ; node = node->next
    mov     ecx, dword [ecx + Node.next]
    cmp     ecx, esi
    jne     .scale_loop
.scale_loop_end:

    pop     esi
    pop     ebx
    ret
%undef queue
%undef mulval
%undef addval

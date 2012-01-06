;
; interrupt.s -- Contains interrupt service routine wrappers.
;                Based on Bran's kernel development tutorials.
;                Rewritten for JamesM's kernel development tutorials.

; This macro creates a stub for an ISR which does NOT pass it's own
; error code (adds a dummy errcode byte).
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts firstly.
    push byte 0                 ; Push a dummy error code.
    jmp isr_common_stub         ; Go to our common handler code.
%endmacro

; This macro creates a stub for an ISR which passes it's own
; error code.
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts.
    jmp isr_common_stub
%endmacro


GLOBAL idt_flush    ; Allows the C code to call idt_flush().

idt_flush:
   mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
   lidt [eax]        ; Load the IDT pointer.
   ret


ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12

extern isr13_handler

global isr13
isr13:
	pusha
	
    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr13_handler

    pop ebx        ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
	
	
	popa
	add esp, 4 
    sti
    iret


ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
 


; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    add esp, 4     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

;extern main
;SECTION .text 
org 0x7c00
PAYLOAD_BLOCKS equ 0x10


cli
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
;mov esp,0x7b00
;mov ebp,esp
sti
mov al, PAYLOAD_BLOCKS
mov ah, 02h
mov bx,my_code ;  my_code
mov cx, 0002h
int 0x13

   lgdt [gdt_size]	
   mov eax, cr0
   xor eax, 1
   mov cr0, eax
   nop
jmp	0x8:_run
_run:
	bits 32	
	mov ax, 16
	mov ds, ax
	mov es, ax
	mov gs, ax	
	add ax,8
	mov ss, ax	
	mov esp, 0xffff
	call my_code
_stop_loop:
	cli
	hlt
	jmp _stop_loop


gdt_size: dw 40d
gdt_offset: dd gdt_0


gdt_0: dq 0
gdt_code: dq 0x00C09A0000007FFF
gdt_data: dq 0x00C0920000007FFF
gdt_stack: dq 0x00C0920000007FFF
gdt_tss:  dq 0x01408900000000FF
message		db	'TEST',0
times 510-($-$$) db 0
db 0x55, 0xaa
my_code:

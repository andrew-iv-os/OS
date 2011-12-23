org 0x7c00

cli
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
;mov esp,0x7b00
;mov ebp,esp
sti
mov al, 1
mov ah, 02h
mov bx,0x7e00 ;  my_code
mov cx, 0002h
int 0x13

   lgdt [gdt_size]
	
   mov eax, cr0
   xor eax, 1
   mov cr0, eax 

	jmp	8:_run
_run:
	
	bits 32
	;mov ax,8
	;mov cs,ax
	mov ax, 16
	mov ds, ax
	add ax,8
	mov ss, ax
	mov esp, 0x7fff
	call 0x7e00
_stop_loop:
	cli
	hlt
	jmp _stop_loop


gdt_size: dw 40d
gdt_offset: dd gdt_0


gdt_0: dq 0
gdt_code: dq 0x00CF9A000000FFFF
gdt_data: dq 0x00CF92000000FFFF
gdt_stack: dq 0x00CF92000000FFFF
gdt_tss:  dq 0x01408900000000FF
message		db	'TEST',0
times 510-($-$$) db 0
db 0x55, 0xaa
my_code:

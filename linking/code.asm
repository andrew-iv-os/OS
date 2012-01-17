SECTION .text 
global Fnc
extern Prn

Fnc:
var_10 equ -10h
var_C equ -0Ch
var_A equ -0Ah
var_8 equ -8
arg_0  equ 8
arg_4 equ  0Ch
arg_8 equ  10h

push    ebp
mov     ebp, esp
sub     esp, 10h
mov     eax, [ebp+arg_0]
mov     [ebp+var_8], eax
mov     eax, [ebp+arg_4]
mov     [ebp+var_8+4], eax
fild    dword [ebp+arg_8]
fld     qword [ebp+var_8]
fdivrp 
fld     qword [v12e1]
fmulp  
fnstcw  [ebp+var_A]
movzx   eax, word [ebp+var_A]
mov     ah, 0Ch
mov     [ebp+var_C], ax
fldcw   [ebp+var_C]
fistp   dword [ebp+var_10]
fldcw   [ebp+var_A]
mov     eax, [ebp+var_10]
push eax
call Prn
pop eax
leave
retn

v12e1 dq 1.2e1

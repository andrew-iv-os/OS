gcc -c code.c -o c.o
nasm -f elf32 ./code.asm -o ./asm.o
gcc c.o asm.o -o prog


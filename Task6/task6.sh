gcc -c task6.c -o c.obj
nasm task6.asm -o asm.bin
ld --script align.ld c.obj -o linked_c.obj
objcopy -j .text -O binary linked_c.obj c.bin
cat asm.bin c.bin > ./image.bin

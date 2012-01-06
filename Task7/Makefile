SOURCES=task7.o common.o monitor.o isr.o interrupt.o

CFLAGS=-c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32 -march=i386 
LDFLAGS=--oformat elf32-i386 -m elf_i386 --script align.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o linked.obj b.out

link: boot
	ld $(LDFLAGS) -o linked.obj $(SOURCES)
	objcopy -O binary linked.obj b.out
	cat boot.bin b.out > ./img.bin

boot:
	nasm boot.asm -o boot.bin


.c.o:
	gcc $(CFLAGS) $<

.s.o:
	nasm $(ASFLAGS) $<



#C_SOURCES=task7.c
#all: 
#	gcc -m32 -march=i386 -c $(C_SOURCES) -o c.obj
#	nasm boot.asm -o boot.bin
#	#nasm -f elf32 ./task7.asm -o ./asm.o
#	ld -m elf_i386 --oformat=elf32-i386 --script align.ld c.obj -o linked_c.obj
#	objcopy -j .text -O binary linked_c.obj ./c.bin
#	cat boot.bin c.bin > ./img.bin

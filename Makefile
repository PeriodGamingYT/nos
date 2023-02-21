SOURCES= \
	boot.o \
	common.o \
	vga.o \
	tables_stub.o \
	int_stub.o \
	isr.o \
	tables.o \
	timer.o \
	key.o \
	main.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-Tlink.ld -m elf_i386
ASFLAGS=-felf
all: $(SOURCES) link
clean:
	-rm *.o kernel.bin

qemu: kernel.bin
	qemu-system-i386 -kernel kernel.bin

link:
	clear
	ld $(LDFLAGS) -o kernel.bin $(SOURCES)
	make qemu

.s.o:
	nasm $(ASFLAGS) $<

SOURCES= \
	boot.o \
	common.o \
	vga.o \
	tables_stub.o \
	int_stub.o \
	isr.o \
	tables.o \
	vm.o \
	timer.o \
	key.o \
	kheap.o \
	paging.o \
	ordered_array.o \
	paging_stub.o \
	main.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-Tlink.ld -m elf_i386
ASFLAGS=-felf
all: $(SOURCES) link
clean:
	rm -rf *.o kernel.bin iso

qemu: kernel.bin
	qemu-system-i386 -kernel kernel.bin -no-reboot

link:
	clear
	ld $(LDFLAGS) -o kernel.bin $(SOURCES)
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp grub.cfg iso/boot/grub
	cp kernel.bin iso/boot
	grub-mkrescue --output=nos.iso iso
	rm -rf iso
	make qemu

.s.o:
	nasm $(ASFLAGS) $<

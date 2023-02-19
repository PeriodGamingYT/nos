GCC_FLAGS = -m32 -ffreestanding -std=gnu99 -O2 -I.
LD_FLAGS = -ffreestanding -O2 -nostdlib
make:
	clear
	rm -f *.o
	rm -f *.a
	rm -f *.iso
	rm -f *.bin
	rm -rf bin iso
	as --32 boot.s -o boot.o
	as --32 int_stub.s -o int_stub.o
	gcc $(GCC_FLAGS) -c kernel.c -o kernel.o
	gcc $(GCC_FLAGS) -c vga.c -o vga.o
	gcc $(GCC_FLAGS) -c gdt.c -o gdt.o
	gcc $(GCC_FLAGS) -c int.c -o int.o
	gcc $(GCC_FLAGS) -c port.c -o port.o

	gcc -T link.ld -o nos.bin $(LD_FLAGS) \
		boot.o \
		gdt.o \
		port.o \
		int_stub.o \
		int.o \
		vga.o \
		kernel.o \

	mkdir -p iso/boot/grub
	cp nos.bin iso/boot/nos.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o nos.iso iso
	qemu-system-i386 -cdrom nos.iso

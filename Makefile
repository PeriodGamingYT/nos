GCC_FLAGS = -m32 -ffreestanding -std=gnu99 -O2
LD_FLAGS = -ffreestanding -O2 -nostdlib
make:
	clear
	rm -f *.o
	rm -f *.a
	rm -rf bin iso
	as --32 boot.s -o boot.o
	gcc $(GCC_FLAGS) -c kernel.c -o kernel.o
	gcc $(GCC_FLAGS) -c vga.c -o vga.o
	gcc -T link.ld -o nos.bin $(LD_FLAGS) \
		boot.o \
		vga.o \
		kernel.o \

	mkdir -p iso/boot/grub
	cp nos.bin iso/boot/nos.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o nos.iso iso
	qemu-system-i386 -cdrom nos.iso

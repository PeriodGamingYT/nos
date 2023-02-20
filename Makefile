GCC_FLAGS = -m32 -ffreestanding -std=gnu99 -I.
LD_FLAGS = -ffreestanding -nostdlib -no-pie
make:
	clear
	rm -f *.o
	rm -f *.a
	rm -f *.iso
	rm -f *.bin
	rm -f *.out
	rm -rf bin iso
	as --32 boot.s -o boot.o
	gcc $(GCC_FLAGS) -c kernel.c -o kernel.o
	gcc $(GCC_FLAGS) -c vga.c -o vga.o
	gcc $(GCC_FLAGS) -c gdt.c -o gdt.o
	gcc -T link.ld -o nos.bin $(LD_FLAGS) \
		boot.o \
		gdt.o \
		vga.o \
		kernel.o \

	mkdir -p iso/boot/grub
	cp nos.bin iso/boot/nos.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o nos.iso iso
	qemu-system-i386 -cdrom nos.iso -no-reboot -d int,cpu_reset

clean:
	clear
	rm -f *.o
	rm -f *.a
	rm -f *.iso
	rm -f *.bin
	rm -f *.out
	rm -rf bin iso

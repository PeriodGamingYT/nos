#include "vga.h"
#include "tables.h"

void main(struct multiboot *multiboot_ptr) {
	init_tables();
	print_clear();
	println("nos started");
	asm volatile("int $0x3");
	asm volatile("int $0x4");
}

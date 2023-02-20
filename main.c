#include "vga.h"
#include "tables.h"

void main(struct multiboot *multiboot_ptr) {
	init_tables();
	print_clear();
	println("nos started");
}

#include "vga.h"
#include "tables.h"
#include "key.h"
#include "timer.h"

void main(struct multiboot *multiboot_ptr) {
	init_tables();
	print_clear();
	println("nos started");
	timer_install(50);
	keyboard_install();
	asm volatile("sti");
}

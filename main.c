#include "vga.h"
#include "tables.h"
#include "key.h"
#include "timer.h"
#include "page.h"

void main(struct multiboot *multiboot_ptr) {
	init_tables();
	print_clear();
	timer_install(50);
	keyboard_install();
	page_install();
	asm volatile("sti");
	unsigned int *pointer = (unsigned int *) 0xa0000000;
	unsigned int do_page_fault = *pointer;
	println("nos started");
}

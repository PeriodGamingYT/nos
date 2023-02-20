#include <vga.h>
#include <gdt.h>

void kernel_main() {
	gdt_install();
	terminal_clear();
	print("nos started");
	for(;;);
}

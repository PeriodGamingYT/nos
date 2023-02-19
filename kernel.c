#include <vga.h>
#include <gdt.h>

void kernel_main() {
	terminal_clear();
	struct gdt kernel_gdt = gdt_make();
	print("nos started");
}

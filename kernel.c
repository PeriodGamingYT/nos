#include <vga.h>
#include <gdt.h>
#include <int.h>

void kernel_main() {
	terminal_clear();
	struct gdt kernel_gdt = gdt_make();
	struct int_manager kernel_int_manager = 
		int_manager_make(&kernel_gdt);

	int_activate(&kernel_int_manager, &kernel_gdt);
	print("nos started");
	for(;;);
}

#include "isr.h"
#include "vga.h"

void isr_handler(struct regs arg_regs) {
	print("Int: ");
	print_num(arg_regs.int_no);
	println("!");
}

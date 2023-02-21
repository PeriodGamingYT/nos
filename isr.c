#include "isr.h"
#include "vga.h"
#include "common.h"

isr_t int_handlers[256];
void isr_handler(struct regs arg_regs) {
	unsigned char int_num = arg_regs.int_num & 0xff;
	if(int_handlers[int_num] != 0) {
		isr_t handler = int_handlers[int_num];
		handler(&arg_regs);
		return;
	}

	print("int ");
	print_num(arg_regs.int_num);
	println(". halting");
	for(;;);
}

void irq_handler(struct regs arg_regs) {
	if(arg_regs.int_num >= 40) {
		out_8(0xa0, 0x20);
	}

	out_8(0x20, 0x20);
	if(int_handlers[arg_regs.int_num] != 0) {
		isr_t handler = int_handlers[arg_regs.int_num];
		handler(&arg_regs);
	}
}

void register_int_handler(
	unsigned char num, 
	isr_t handler
) {
	int_handlers[num] = handler;
}

#include "isr.h"
#include "vga.h"

void isr_handler(struct regs *arg_regs) {
	println("Int!");
}

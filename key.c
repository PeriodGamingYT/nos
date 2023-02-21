#include "key.h"
#include "isr.h"
#include "common.h"
#include "vga.h"

void keyboard_callback(struct regs *arg_regs) {
	unsigned char scancode = in_8(0x60);
	print("key ");
	print_num(scancode);
	println("");
}

void keyboard_install() {
	register_int_handler(IRQ_NUM(1), (isr_t) keyboard_callback);
}

#include "timer.h"
#include "isr.h"
#include "common.h"
#include "vga.h"

unsigned int tick = 0;
void (*handler)();
static void timer_callback(struct regs *arg_regs) {
	tick++;
	if(handler != 0) {
		handler();
	}
	
	#ifdef COMMON_DEBUG_TIMER
		print("tick ");
		print_num(tick);
		println("");
	#endif
}

void timer_handler_set(void (*arg_handler)()) {
	handler = arg_handler;
}

void timer_delay(unsigned int num_ticks) {
	unsigned int stop_ticks = tick + num_ticks;
	while(tick < stop_ticks);
}

void timer_install(unsigned int freq) {
	register_int_handler(IRQ_NUM(0), &timer_callback);
	unsigned int divisor = 1193180 / freq;
	out_8(0x43, 0x36);
	unsigned char low = (unsigned char)(divisor & 0xff);
	unsigned char high = (unsigned char)((divisor >> 8) & 0xff);
	out_8(0x40, low);
	out_8(0x80, high);
}

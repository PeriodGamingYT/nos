#ifndef __ISR_H
#define __ISR_H
#define IRQ_NUM(x) ((x) + 32)
struct regs {
	unsigned int ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_num, error_code;
	unsigned int eip, cs, eflags, user_esp, ss;
};


typedef void (*isr_t)(struct regs *);
void register_int_handler(unsigned char, isr_t);
#endif
